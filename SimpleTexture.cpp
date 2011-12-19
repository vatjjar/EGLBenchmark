
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "SimpleTexture.h"
#include "GLWrapper.h"
#include "DebugLog.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>

#include "png.h"

SimpleTexture::SimpleTexture()
{
}

SimpleTexture::~SimpleTexture()
{
    destroy();
}

/******************************************************************************
 * File I/O
 */

bool SimpleTexture::fromFile(const char *filename)
{
    unsigned int l;

    l = strlen(filename);
    if (filename[l-3]=='p' && filename[l-2]=='k' && filename[l-1]=='m')
    {
        DebugLog::Instance()->MESSAGE(2, "Trying to open PKM/ETC1 texture file %s\n", filename);
        texture_id = loadETCTextureFromFile(filename);
        if (texture_id == 0) return false;
    }
    else if (filename[l-3]=='p' && filename[l-2]=='n' && filename[l-1]=='g')
    {
        DebugLog::Instance()->MESSAGE(2, "Trying to open PNG texture file %s\n", filename);
        texture_id = loadRGBTexturefromPNG(filename);
        if (texture_id == 0) return false;
    }
    else
    {
        DebugLog::Instance()->MESSAGE(2, "File %s nto recognized. Abort!\n", filename);
        return false;
    }
    return true;
}

void SimpleTexture::bind(void)
{
    GLWrapper::Instance()->GLBINDTEXTURE(GL_TEXTURE_2D, texture_id);
}

void SimpleTexture::destroy(void)
{
    if (texture_id != 0)
        GLWrapper::Instance()->GLDELETETEXTURES(1, &texture_id);
}

/******************************************************************************
 * Private methods
 */

/*
 * File I/O helper
 * ---------------
 */

unsigned char * SimpleTexture::readBinaryFile(const char *filename, unsigned int & length)
{
    struct stat results;
    unsigned char * buffer;

    if (stat(filename, &results) != 0)
    {
        DebugLog::Instance()->MESSAGE(4, "IOError when trying to access '%s'\n", filename);
        return NULL;
    }
    buffer = new unsigned char [results.st_size];
    length = results.st_size;

    DebugLog::Instance()->MESSAGE(4, "Trying to open '%s'\n", filename);
    std::ifstream f(filename, std::ios::in | std::ios::binary);
    if (!f.read ((char *)buffer, results.st_size))
    {
        DebugLog::Instance()->MESSAGE(4, "IOERROR\n");
        return NULL;
    }
    f.close();
    return buffer;
}

/*
 * Texture Loader helpers
 * ----------------------
 */

GLuint SimpleTexture::loadETCTextureFromFile(const char *filename)
{
#if defined(GL_ETC1_RGB8_OES)
    unsigned int length;
    unsigned char *buffer;
    GLuint textureID;
    unsigned short w, h;

    buffer = readBinaryFile(filename, length);
    if (buffer == NULL)
    {
        return 0;
    }
    DebugLog::Instance()->MESSAGE(4, "PKM File read into memory, length %d bytes\n", (int)length);

    w = (buffer[8] << 8) + buffer[9];
    h = (buffer[10] << 8) + buffer[11];
    DebugLog::Instance()->MESSAGE(4, "ETC1 image width %d and height %d\n", w, h);

    // GL texture generation part
    GLWrapper::Instance()->GLGENTEXTURES(1, &textureID);
    GLWrapper::Instance()->GLBINDTEXTURE(GL_TEXTURE_2D, textureID);

    GLWrapper::Instance()->GLCOMPRESSEDTEXIMAGE2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, w, h, 0, length-16, &buffer[16]);
    delete buffer;

    // This is critical. CompressedTexImage may return invalid ENUM and if so, we will cancel
    if (GLWrapper::Instance()->getGLErrors() != 0)
    {
        DebugLog::Instance()->MESSAGE(4, "Texture loading aborted due to errors in glCompressedTexImage2D()\n");
        GLWrapper::Instance()->GLDELETETEXTURES(1, &textureID);
        return 0;
    }

    GLWrapper::Instance()->GLTEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GLWrapper::Instance()->GLTEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureID;
#else
    DebugLog::Instance()->MESSAGE(4, "ETC1 Texture loading not supported by this platform\n");
    DebugLog::Instance()->MESSAGE(4, "Preprocessor flag GL_ETC1_RGB8_OES not defined\n");
    return 0;
#endif
}

GLuint SimpleTexture::loadRGBTexturefromPNG(const char *filename)
{
    /*
     * Following loader based on tips and tricks on Morten Noble's blog
     * http://blog.nobel-joergensen.com/2010/11/07/loading-a-png-as-texture-in-opengl-using-libpng
     */
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    FILE *fp;
    unsigned int width, height;
    bool hasAlpha;
    unsigned char *buffer;
    GLuint textureID;

    if ((fp = fopen(filename, "rb")) == NULL)
        return false;

    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);

    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }

    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
        return 0;
    }

    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return 0;
    }

    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);

    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, png_voidp_NULL);

    width = info_ptr->width;
    height = info_ptr->height;
    switch (info_ptr->color_type) {
        case PNG_COLOR_TYPE_RGBA:
            hasAlpha = true;
            break;
        case PNG_COLOR_TYPE_RGB:
            hasAlpha = false;
            break;
        default:
            DebugLog::Instance()->MESSAGE(4, "PNG: color type %d not supported by loader.\n", info_ptr->color_type);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return 0;
    }
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    buffer = new unsigned char [row_bytes * height];

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    for (unsigned int i = 0; i < height; i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(buffer+(row_bytes * (height-1-i)), row_pointers[i], row_bytes);
    }

    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

    /* Close the file */
    fclose(fp);
    DebugLog::Instance()->MESSAGE(4, "PNG image loaded: %dx%d alpha:%d\n", width, height, hasAlpha);


    /*
     * GL texture generation part
     */
    GLWrapper::Instance()->GLGENTEXTURES(1, &textureID);
    GLWrapper::Instance()->GLBINDTEXTURE(GL_TEXTURE_2D, textureID);
    GLWrapper::Instance()->GLPIXELSTOREI(GL_UNPACK_ALIGNMENT, 1);

    GLWrapper::Instance()->GLTEXIMAGE2D(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB, width,
                 height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
                 buffer);

    // At this point we might have errors already in the pipe, and if so, we'll cancel
    if (GLWrapper::Instance()->getGLErrors() != 0)
    {
        DebugLog::Instance()->MESSAGE(4, "Texture loading aborted due to errors in glTexImage2D()\n");
        GLWrapper::Instance()->GLDELETETEXTURES(1, &textureID);
        return 0;
    }

    GLWrapper::Instance()->GLTEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GLWrapper::Instance()->GLTEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureID;
}
