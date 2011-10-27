
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "EGLX11Benchmark.h"

#include <vector>
#include <iostream>
#include <fstream>
#include "sys/stat.h"

#include "png.h"

EGLX11Benchmark::EGLX11Benchmark() :
    name("No name"),
    description("No description"),
    win(0),
    x_display(NULL),
    egl_display(0),
    egl_surface(0),
    egl_context(0),
    verbosity(1),
    GLerrors(0),
    EGLerrors(0)
{
    // dummy constructor
}

EGLX11Benchmark::~EGLX11Benchmark()
{
}

const char * EGLX11Benchmark::getName(void)
{
    return name;
}

const char * EGLX11Benchmark::getDescription(void)
{
    return description;
}

void EGLX11Benchmark::setName(const char *n)
{
    name = n;
}

void EGLX11Benchmark::setDescription(const char *d)
{
    description = d;
}

void EGLX11Benchmark::outputMessage(int level, const char *message)
{
    if (verbosity >= level)
    {
        std::cout << level <<": " << message;
    }
}

void EGLX11Benchmark::flushGLErrors(void)
{
    GLuint error;
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        GLerrors++;
        std::cout << "GL ERROR: glGetError() returned " << error;
        switch(error)
        {
        case GL_INVALID_VALUE:
            std::cout << " (GL_INVALID_VALUE)\n";
            break;
        case GL_INVALID_ENUM:
            std::cout << " (GL_INVALID_ENUM)\n";
            break;
        case GL_INVALID_OPERATION:
            std::cout << " (GL_INVALID_OPERATION)\n";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << " (GL_INVALID_FRAMEBUFFER_OPERATION) ";
            std::cout << "glCheckFramebufferStatus(GL_FRAMEBUFFER) = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n";
            break;
        default:
            std::cout << " (UNKNOWN GL ERROR)\n";
            break;
        }
    }
}

void EGLX11Benchmark::flushEGLErrors(void)
{
    GLuint error;
    error = eglGetError();
    if (error != EGL_SUCCESS)
    {
        EGLerrors++;
        std::cout << "EGL ERROR: eglGetError() returned " << error;
        switch(error)
        {
        case EGL_NOT_INITIALIZED:
            std::cout << " (EGL_NOT_INITIALIZED)\n";
            break;
        case EGL_BAD_ACCESS:
            std::cout << " (EGL_BAD_ACCESS)\n";
            break;
        case EGL_BAD_ALLOC:
            std::cout << " (EGL_BAD_ALLOC)\n";
            break;
        case EGL_BAD_ATTRIBUTE:
            std::cout << " (EGL_BAD_ATTRIBUTE) ";
            break;
        case EGL_BAD_CONTEXT:
            std::cout << " (EGL_BAD_CONTEXT) ";
            break;
        case EGL_BAD_CONFIG:
            std::cout << " (EGL_BAD_CONFIG) ";
            break;
        case EGL_BAD_CURRENT_SURFACE:
            std::cout << " (EGL_BAD_CURRENT_SURFACE) ";
            break;
        case EGL_BAD_DISPLAY:
            std::cout << " (EGL_BAD_DISPLAY) ";
            break;
        case EGL_BAD_SURFACE:
            std::cout << " (EGL_BAD_SURFACE) ";
            break;
        case EGL_BAD_MATCH:
            std::cout << " (EGL_BAD_MATCH) ";
            break;
        case EGL_BAD_PARAMETER:
            std::cout << " (EGL_BAD_PARAMETER) ";
            break;
        case EGL_BAD_NATIVE_PIXMAP:
            std::cout << " (EGL_BAD_NATIVE_PIXMAP) ";
            break;
        case EGL_BAD_NATIVE_WINDOW:
            std::cout << " (EGL_BAD_NATIVE_WINDOW) ";
            break;
        default:
            std::cout << " (UNKNOWN EGL ERROR)\n";
            break;
        }
    }
}

void EGLX11Benchmark::setVerbosityLevel(unsigned int level)
{
    verbosity = level;
}

void EGLX11Benchmark::resetTimer(void)
{
    //// this is needed for time measuring  -->  frames per second
    gettimeofday ( &t1 , &tz );
    t2 = t1;
}

float EGLX11Benchmark::getTime(void)
{
    // Calculate elapsed time since last timer reset (comparing to t1 timestamp)
    // Result returned in seconds (float)
    gettimeofday(&t2, &tz);
    return (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
}

bool EGLX11Benchmark::userInterrupt(void)
{
    XEvent  xev;
    bool userinterrupt = false;

    // Pump all messages from X server and interpret keypresses as user interrupts
    // No other intelligence here whatsoever
    while ( XPending ( x_display ) )
    {
        XNextEvent( x_display, &xev );
        if ( xev.type == KeyPress )
            userinterrupt = true;
    }
    return userinterrupt;
}

unsigned int EGLX11Benchmark::getGLErrors(void)
{
    return GLerrors;
}

unsigned int EGLX11Benchmark::getEGLErrors(void)
{
    return EGLerrors;
}

////////////////////////////////////////////////////////////////////////////////////////////

bool EGLX11Benchmark::createEGLDisplay(int width, int height, bool fullscreen)
{
   Window root;
   XSetWindowAttributes swa;
   XSetWindowAttributes  xattr;
   Atom wm_state, a_fullscreen;
   XWMHints hints;
   XEvent xev;
   EGLConfig ecfg;
   EGLint num_config;

    w_width = width;
    w_height = height;
    w_fullscreen = fullscreen;

   /*
    * X11 native display initialization
    */

   MESSAGE(3, "Connecting to X server\n");
   x_display = XOpenDisplay(NULL);
   if ( x_display == NULL )
   {
       MESSAGE(1, "Error: Unable to connect to X Server\n");
       return false;
   }

   MESSAGE(3, "Querying X root window\n");
   root = DefaultRootWindow(x_display);

   MESSAGE(3, "Creating X11 window\n");
   swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
   win = XCreateWindow(
              x_display, root,
              0, 0, width, height, 0,
              CopyFromParent, InputOutput,
              CopyFromParent, CWEventMask,
              &swa );

   MESSAGE(3, "Updating window attributes\n");
   xattr.override_redirect = false;
   XChangeWindowAttributes ( x_display, win, CWOverrideRedirect, &xattr );

   MESSAGE(3, "Setting Window manager hints\n");
   hints.input = true;
   hints.flags = InputHint;
   XSetWMHints(x_display, win, &hints);

   // make the window visible on the screen
   MESSAGE(3, "Making window visible\n");
   XMapWindow (x_display, win);
   XStoreName (x_display, win, "EGLX11Benchmark");

   // get identifiers for the provided atom name strings
   wm_state = XInternAtom (x_display, "_NET_WM_STATE", false);
   a_fullscreen = false;
   if (w_fullscreen == true)
       a_fullscreen = XInternAtom (x_display, "_NET_WM_STATE_FULLSCREEN", w_fullscreen);

   MESSAGE(3, "Updating window event masks\n");
   memset ( &xev, 0, sizeof(xev) );
   xev.type                 = ClientMessage;
   xev.xclient.window       = win;
   xev.xclient.message_type = wm_state;
   xev.xclient.format       = 32;
   xev.xclient.data.l[0]    = 1;
   xev.xclient.data.l[1]    = a_fullscreen;
   XSendEvent (
      x_display,
      DefaultRootWindow ( x_display ),
      false,
      SubstructureNotifyMask,
      &xev );

   MESSAGE(2, "X11 native display init done!\n");

   /*
    * Now that the native window is up, we shall initialize EGL
    */

   MESSAGE(3, "EGL: eglGetDisplay()\n");
   egl_display  =  eglGetDisplay( (EGLNativeDisplayType) x_display );
   if ( egl_display == EGL_NO_DISPLAY ) {
       flushEGLErrors();
       MESSAGE(1, "EGL: eglGetDisplay() failed!\n");
       return false;
   }
   flushEGLErrors();

   MESSAGE(3, "EGL: eglInitialize()\n");
   if ( !eglInitialize( egl_display, NULL, NULL ) ) {
       flushEGLErrors();
       MESSAGE(1, "EGL: eglInitialize() failed!\n");
       return false;
   }
   flushEGLErrors();

   MESSAGE(3, "EGL: eglChooseConfig()\n");
   if ( !eglChooseConfig( egl_display, attr, &ecfg, 1, &num_config ) ) {
       flushEGLErrors();
       outputMessage(1, "EGL: eglChooseConfig() failed!\n");
       return false;
   }
   flushEGLErrors();

   MESSAGE(3, "EGL: EGL configs available?\n");
   if ( num_config == 0 ) {
       flushEGLErrors();
       MESSAGE(1, "EGL: eglGetDisplay() no configs found!\n");
       return false;
   }
   flushEGLErrors();

   MESSAGE(3, "EGL: eglCreateWindowSurface()\n");
   egl_surface = eglCreateWindowSurface ( egl_display, ecfg, win, NULL );
   if ( egl_surface == EGL_NO_SURFACE ) {
       flushEGLErrors();
       MESSAGE(5, "EGL: eglCreateWindowSurface() failed!\n");
       return false;
   }
   flushEGLErrors();

   MESSAGE(3, "EGL: eglCreateContext()\n");
   egl_context = eglCreateContext ( egl_display, ecfg, EGL_NO_CONTEXT, ctxattr );
   if ( egl_context == EGL_NO_CONTEXT ) {
       flushEGLErrors();
       MESSAGE(1, "EGL: eglCreateContext() failed!\n");
       return false;
   }
   flushEGLErrors();

   MESSAGE(3, "EGL: eglMakeCurrent()\n");
   eglMakeCurrent( egl_display, egl_surface, egl_surface, egl_context );
   flushEGLErrors();
   flushGLErrors();

   MESSAGE(2, "EGL initialization completed!\n");

   return true;
}

void EGLX11Benchmark::destroyEGLDisplay(void)
{
    MESSAGE(3, "Starting EGL/X11 destruction\n");
    if (egl_display != 0)
    {
        if (egl_context != 0)
            eglDestroyContext (egl_display, egl_context);
        if (egl_surface != 0)
            eglDestroySurface (egl_display, egl_surface);
        eglTerminate (egl_display);
    }
    if (x_display != NULL)
    {
        if (win != 0)
            XDestroyWindow(x_display, win);
        XCloseDisplay(x_display);
    }
    egl_display = 0;
    egl_context = 0;
    egl_surface = 0;
    win = 0;
    x_display = NULL;
    outputMessage(3, "EGL/X11 Destruction done\n");
}

// Shader helpers

void EGLX11Benchmark::printShaderInfo ( GLuint shader )
{
   GLint length;

   MESSAGE(4, "printShaderInfo: glGetShaderiv()\n");
   glGetShaderiv ( shader , GL_INFO_LOG_LENGTH , &length );
   flushGLErrors();

   if ( length ) {
      char buffer[length];
      GLint success;

      MESSAGE(4, "printShaderInfo: glGetShaderInfoLog()\n");
      glGetShaderInfoLog ( shader , length , NULL , buffer );
      flushGLErrors();
      MESSAGE_1P(4, "Shader info: '%s'\n", buffer);

      MESSAGE(4, "printShaderInfo: glGetShaderiv()\n");
      glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
      flushGLErrors();
      if ( success != GL_TRUE )
      {
          MESSAGE(4, "Error: Shader compile status failed\n");
          return;
      }
      else MESSAGE(4, "printShaderInfo: OK\n");
   }
}

/*
 * Shader program helpers
 * ----------------------
 */

GLuint EGLX11Benchmark::loadShaderProgram ( const char *shader_source, GLenum type)
{
   GLuint shader;

   MESSAGE(4, "loadShader: glCreateShader()\n");
   shader = glCreateShader( type );
   flushGLErrors();
   if (shader == 0)
   {
       return 0;
   }
   MESSAGE_1P(4, "loadShader: shader handle = %d\n", shader);

   MESSAGE(4, "loadShader: glShaderSource()\n");
   glShaderSource  ( shader , 1 , &shader_source , NULL );
   flushGLErrors();

   MESSAGE(4, "loadShader: glCompileShader()\n");
   glCompileShader ( shader );
   flushGLErrors();

   MESSAGE(4, "loadShader: Logging info...\n");
   printShaderInfo ( shader );

   MESSAGE(4, "loadShader: EXIT\n");
   return shader;
}

GLuint EGLX11Benchmark::createShaderProgram(const char *v_src, const char *f_src)
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;

    shaderProgram = GLCREATEPROGRAM();
    if (shaderProgram == 0)
    {
        MESSAGE(1, "Error: Shader program creation failed\n");
        return 0;
    }

    MESSAGE(4, "Initializing shaders...\n");
    if (v_src == NULL || f_src == NULL)
    {
        MESSAGE(4, "Vertex nor fragment shader source must not be NULL.\n");
        return 0;
    }

    vertexShader   = loadShaderProgram ( v_src , GL_VERTEX_SHADER  );
    fragmentShader = loadShaderProgram ( f_src , GL_FRAGMENT_SHADER );
    if (vertexShader == 0 || fragmentShader == 0)
    {
        MESSAGE(1, "Error: Shader program loading failed\n");
        return 0;
    }

    GLATTACHSHADER(shaderProgram, vertexShader);
    GLATTACHSHADER(shaderProgram, fragmentShader);

    return shaderProgram;
}

void EGLX11Benchmark::linkShaderProgram(GLuint shaderProgram)
{
    GLLINKPROGRAM(shaderProgram);
    GLUSEPROGRAM(shaderProgram);
}

/*
 * File I/O helper
 * ---------------
 */

unsigned char * EGLX11Benchmark::readBinaryFile(const char *filename, unsigned int & length)
{
    struct stat results;
    unsigned char * buffer;

    if (stat(filename, &results) != 0)
    {
        MESSAGE_1P(4, "IOError when trying to access '%s'\n", filename);
        return NULL;
    }
    buffer = new unsigned char [results.st_size];
    length = results.st_size;

    MESSAGE_1P(4, "Trying to open '%s'\n", filename);
    std::ifstream f(filename, std::ios::in | std::ios::binary);
    if (!f.read ((char *)buffer, results.st_size))
    {
        MESSAGE(4, "IOERROR\n");
        return NULL;
    }
    f.close();
    return buffer;
}

/*
 * Texture Loader helpers
 * ----------------------
 */

GLuint EGLX11Benchmark::loadETCTextureFromFile(const char *filename)
{
    unsigned int length;
    unsigned char *buffer;
    GLuint textureID;

    buffer = readBinaryFile(filename, length);
    if (buffer == NULL)
    {
        return 0;
    }
    MESSAGE_1P(4, "File read into memory, length %d bytes\n", (int)length);

    // GL texture generation part
    glGenTextures(1, &textureID);
    flushGLErrors();
    glBindTexture(GL_TEXTURE_2D, textureID);
    flushGLErrors();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    flushGLErrors();
    delete buffer;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    flushGLErrors();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    flushGLErrors();

    return textureID;
}

GLuint EGLX11Benchmark::loadRGBTexturefromPNG(const char *filename)
{
    /*
     * Following loader based on tips and tricks on Morten Noble's blog
     * http://blog.nobel-joergensen.com/2010/11/07/loading-a-png-as-texture-in-opengl-using-libpng
     */
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    //int color_type, interlace_type;
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
            MESSAGE_1P(4, "PNG: color type %d not supported by loader.\n", info_ptr->color_type);
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


    //std::cout << "Image loaded " << width << " " << height << " alpha " << hasAlpha << std::endl;
    MESSAGE_3P(4, "PNG image loaded: %dx%d alpha:%d\n", width, height, hasAlpha);

#if 0
    // GL texture generation part
    glGenTextures(1, &textureID);
    flushGLErrors();
    glBindTexture(GL_TEXTURE_2D, textureID);
    flushGLErrors();

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    flushGLErrors();
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, width,
            height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
            buffer);
    flushGLErrors();
    delete buffer;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    flushGLErrors();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    flushGLErrors();
#else
    GLGENTEXTURES(1, &textureID);
    GLBINDTEXTURE(GL_TEXTURE_2D, textureID);
    GLPIXELSTOREI(GL_UNPACK_ALIGNMENT, 1);
    GLTEXIMAGE2D(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB, width,
                 height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
                 buffer);
    GLTEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GLTEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
    return textureID;
}
