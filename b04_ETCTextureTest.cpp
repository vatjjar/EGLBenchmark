
/*
 * b04_ETCTextureTest - benchmark #4, ETC Texture test:
 * ----------------------------------------------------
 * This test case tests the ETC1 texturing method on a quadrilateral rendering
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "b04_ETCTextureTest.h"

#include "DebugLog.h"
#include "GLWrapper.h"

/*
 * Constructor and destructor are dummy ones. Only descriptions are set, and other activities are
 * done when calling the virtual benchmark API
 */

b04_ETCTextureTest::b04_ETCTextureTest()
{
    setName("ETC1 texture mapping test");
    setDescription("This test tests texture compression parts of GLES2 and executes a case using ETC texture format");
}

b04_ETCTextureTest::~b04_ETCTextureTest()
{
}

/*
 * initBenchmark() shall initialize all required resources for this test case. If initialization fails,
 * false must be returned to indicate core benchmark not to continue execution. Parent class DebugLog::Instance()->MESSAGE()
 * method can be used to output information about the initialization
 */
bool b04_ETCTextureTest::initBenchmark(void)
{
    const char *texturefilename = "./resources/etctexture.pkm";
    const char vertex_src[] =
       "attribute vec4 a_Position;   \n"
       "attribute vec2 a_Texcoord;   \n"
       "varying vec2 v_Texcoord;     \n"
       "void main()                  \n"
       "{                            \n"
       "   gl_Position = a_Position; \n"
       "   v_Texcoord = a_Texcoord;  \n"
       "}                            \n";
    const char fragment_src[] =
       "precision mediump float;                     \n"
       "varying vec2 v_Texcoord;                     \n"
       "uniform sampler2D s_texture;                 \n"
       "void main()                                  \n"
       "{                                            \n"
       "  gl_FragColor = texture2D(s_texture, v_Texcoord);\n"
       "}                                            \n";

    // Check for support of compressed ETC1 texture format
    if (false == queryCompressedTextureformats())
    {
        DebugLog::Instance()->MESSAGE(1, "Error: ETC1 texture format not supported by the driver\n");
        return false;
    }

    /*
     * Shader program init:
     */
    ss = new SimpleShader();
    if (false == ss->fromFiles(vertex_src, fragment_src))
    {
        DebugLog::Instance()->MESSAGE(2, "Shader program object creation failed\n");
        return false;
    }

    GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 0, "a_Position");
    GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 1, "a_Texcoord");
    ss->linkProgram();

    texturesampler = GLWrapper::Instance()->GLGETUNIFORMLOCATION(ss->getProgramObject(), "s_texture");

    /*
     * Texture loading for the test case:
     */
    st = new SimpleTexture();
    if (false == st->fromFile(texturefilename))
    {
        DebugLog::Instance()->MESSAGE(1, "Error: Loading of texturefile '%s' failed.\n", texturefilename);
        return false;
    }

    GLWrapper::Instance()->GLCLEARCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

    // If we have errors in GL pipe, then abort.
    if (GLWrapper::Instance()->getGLErrors() > 0) return false;

    return true;
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b04_ETCTextureTest::destroyBenchmark(void)
{
    return true;
}


// Constant vectors for the render test
static GLfloat vVertices[] = {  -0.5f, -0.5f, 0.0f,
                                -0.5f,  0.5f, 0.0f,
                                 0.5f,  0.5f, 0.0f,
                                -0.5f, -0.5f, 0.0f,
                                 0.5f,  0.5f, 0.0f,
                                 0.5f, -0.5f, 0.0f };
static GLfloat vTexcoord[] = { 0.0f, 0.0f,
                               0.0f, 1.0f,
                               1.0f, 1.0f,
                               0.0f, 0.0f,
                               1.0f, 1.0f,
                               1.0f, 0.0f };

void b04_ETCTextureTest::Render(void)
{
    GLWrapper::Instance()->GLVIEWPORT(0, 0, display->getDisplayWidth(), display->getDisplayHeight());
    GLWrapper::Instance()->GLCLEAR(GL_COLOR_BUFFER_BIT);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(0);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(1, 2, GL_FLOAT, GL_FALSE, 0, vTexcoord);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(1);

    GLWrapper::Instance()->GLACTIVETEXTURE(GL_TEXTURE0);
    st->bind();
    GLWrapper::Instance()->GLUNIFORM1I(texturesampler, 0);

    GLWrapper::Instance()->GLDRAWARRAYS(GL_TRIANGLES, 0, 6);

    // get the rendered buffer to the screen
    GLWrapper::Instance()->EGLSWAPBUFFERS(display->getEGLDisplay(), display->getEGLSurface());
}



/*
 * renderSingleFrame()
 */
bool b04_ETCTextureTest::renderSingleFrame(float deltatime)
{
    Render();
    return true;
}

/*
 * getRenderStatistics()
 */
bool b04_ETCTextureTest::getRenderStatistics(RENDER_STATISTICS *rs)
{
    return true;
}



bool b04_ETCTextureTest::queryCompressedTextureformats(void)
{
    GLint *v;
    GLint t;
    bool etc1_supported = false;

    // First we query the list of supported compressed texture formats
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &t);
    GLWrapper::Instance()->flushGLErrors();
    DebugLog::Instance()->MESSAGE(1, "Number of compressed texture formats supported by the driver: %d\n", t);
    if (t == 0)
    {
        DebugLog::Instance()->MESSAGE(1, "Error: The driver does not support texture compression.\n");
        return false;
    }

    v = new GLint [t];

    glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, v);
    GLWrapper::Instance()->flushGLErrors();

    DebugLog::Instance()->MESSAGE(2, "Supported compressed texture formats:\n");
    for (int i=0; i<t; i++)
    {
        DebugLog::Instance()->MESSAGE(2, "format %d: 0x%x ", i, v[i]);
        switch(v[i])
        {
#if defined(GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT: /* 0x83f0 */
            DebugLog::Instance()->MESSAGE(2, "(GL_COMPRESSED_RGB_S3TC_DXT1_EXT)\n");
            break;
#endif
#if defined(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: /* 0x83f1 */
            DebugLog::Instance()->MESSAGE(2, "(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)\n");
            break;
#endif
#if defined(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
        case GL_COMPRESSED_RGB_S3TC_DXT3_EXT: /* 0x83f2 */
            DebugLog::Instance()->MESSAGE(2, "(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)\n");
            break;
#endif
#if defined(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: /* 0x83f3 */
            DebugLog::Instance()->MESSAGE(2, "(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)\n");
            break;
#endif
#if defined(GL_LUMINANCE_LATC1_EXT)
        case GL_LUMINANCE_LATC1_EXT: /* 0x8c70 */
            DebugLog::Instance()->MESSAGE(2, "(GL_LUMINANCE_LATC1_EXT)\n");
            break;
#endif
#if defined(GL_SIGNED_LUMINANCE_LATC1_EXT)
        case GL_SIGNED_LUMINANCE_LATC1_EXT: /* 0x8c71 */
            DebugLog::Instance()->MESSAGE(2, "(GL_SIGNED_LUMINANCE_LATC1_EXT)\n");
            break;
#endif
#if defined(GL_LUMINANCE_ALPHA_LATC2_EXT)
        case GL_LUMINANCE_ALPHA_LATC2_EXT: /* 0x8c72 */
            DebugLog::Instance()->MESSAGE(2, "(GL_LUMINANCE_ALPHA_LATC2_EXT)\n");
            break;
#endif
#if defined(GL_SIGNED_LUMINANCE_ALPHA_LATC2_EXT)
        case GL_SIGNED_LUMINANCE_ALPHA_LATC2_EXT: /* 0x8c73 */
            DebugLog::Instance()->MESSAGE(2, "(GL_SIGNED_LUMINANCE_ALPHA_LATC1_EXT)\n");
            break;
#endif
#if defined(COMPRESSED_SRGB_S3TC_DXT1_EXT)
        case COMPRESSED_SRGB_S3TC_DXT1_EXT: /* 0x8c4c */
            DebugLog::Instance()->MESSAGE(2, "(COMPRESSED_SRGB_S3TC_DXT1_EXT\n");
            break;
#endif
#if defined(COMPRESSED_SRGB_S3TC_DXT1_EXT)
        case COMPRESSED_SRGB_S3TC_DXT1_EXT: /* 0x8c4d */
            DebugLog::Instance()->MESSAGE(2, "(COMPRESSED_SRGB_S3TC_DXT1_EXT)\n");
            break;
#endif
#if defined(COMPRESSED_SRGB_S3TC_DXT1_EXT)
        case COMPRESSED_SRGB_S3TC_DXT1_EXT: /* 0x8c4e */
            DebugLog::Instance()->MESSAGE(2, "(COMPRESSED_SRGB_S3TC_DXT1_EXT)\n");
            break;
#endif
#if defined(COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT)
        case COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT: /* 0x8c4f */
            DebugLog::Instance()->MESSAGE(2, "(COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT)\n");
            break;
#endif
#if defined(GL_ETC1_RGB8_OES)
        case GL_ETC1_RGB8_OES: /* 0x8d64 */
            etc1_supported = true;
            DebugLog::Instance()->MESSAGE(2, "(GL_ETC1_RGB8_OES)\n");
            break;
#endif
        default:
            DebugLog::Instance()->MESSAGE(2, "(UNKNOWN FORMAT)\n");
            break;
        }
    }
    delete v;
    return etc1_supported;
}
