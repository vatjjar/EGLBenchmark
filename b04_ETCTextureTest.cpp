
/*
 * b04_ETCTextureTest - benchmark #1, EGL context creation:
 * -----------------------------------------------------
 * This test case initializes native window, creates render surfaces and associated EGL context
 * to hold all of this information. No other actions are taken. This is the simplest test for EGL
 * framework.
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "b04_ETCTextureTest.h"

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
 * false must be returned to indicate core benchmark not to continue execution. Parent class MESSAGE()
 * method can be used to output information about the initialization
 */
bool b04_ETCTextureTest::initBenchmark(unsigned int width, unsigned int height, bool fullscreen)
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


    // Display and context init
    if (false == createEGLDisplay(width, height, fullscreen))
    {
        return false;
    }

    // Check for support of compressed ETC1 texture format
    if (false == queryCompressedTextureformats())
    {
        MESSAGE(1, "Error: ETC1 texture format not supported by the driver\n");
        return false;
    }

    /*
     * Shader program init:
     */
    shaderProgram = createShaderProgram(vertex_src, fragment_src);
    if (shaderProgram == 0)
    {
        MESSAGE(1, "Error: Shader program creation failed\n");
        return false;
    }
    GLBINDATTRIBLOCATION(shaderProgram, 0, "a_Position");
    GLBINDATTRIBLOCATION(shaderProgram, 1, "a_Texcoord");
    linkShaderProgram(shaderProgram);

    texturesampler = GLGETUNIFORMLOCATION(shaderProgram, "s_texture");

    /*
     * Texture loading for the test case:
     */
    textureID = loadETCTextureFromFile(texturefilename);
    if (textureID == 0)
    {
        MESSAGE(1, "Error: Loading of texturefile '%s' failed.\n", texturefilename);
        return false;
    }

    GLCLEARCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
    return true;
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b04_ETCTextureTest::destroyBenchmark(void)
{
    destroyEGLDisplay();
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
    GLVIEWPORT(0, 0, w_width, w_height);
    GLCLEAR(GL_COLOR_BUFFER_BIT);
    GLUSEPROGRAM(shaderProgram);
    GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    GLENABLEVERTEXATTRIBARRAY(0);
    GLVERTEXATTRIBPOINTER(1, 2, GL_FLOAT, GL_FALSE, 0, vTexcoord);
    GLENABLEVERTEXATTRIBARRAY(1);

    GLACTIVETEXTURE(GL_TEXTURE0);
    GLBINDTEXTURE(GL_TEXTURE_2D, textureID);
    GLUNIFORM1I(texturesampler, 0);

    GLDRAWARRAYS(GL_TRIANGLES, 0, 6);

    EGLSWAPBUFFERS(egl_display, egl_surface);
}

/*
 * runBenchmark()
 */
bool b04_ETCTextureTest::runBenchmark(float duration)
{
    // Timer and variables
    resetTimer();
    totaltime = 0;
    renderedFrames = 0;

    while ( totaltime < duration )
    {
        Render();
        renderedFrames++;

        // Grab time since last timer reset
        totaltime = getTime();

        if (userInterrupt() == true)
            break;
    }
    return true;
}

/*
 * displayResult()
 */
bool b04_ETCTextureTest::displayResult(void)
{
    MESSAGE(1, "Total rendering time %f\n", totaltime);
    MESSAGE(1, "Total rendered frames %d\n", renderedFrames);
    MESSAGE(1, "Frames per second %f\n", renderedFrames/totaltime);
    return true;
}


bool b04_ETCTextureTest::queryCompressedTextureformats(void)
{
    GLint *v;
    GLint t;
    bool etc1_supported = false;

    // First we query the list of supported compressed texture formats
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &t);
    flushGLErrors();
    MESSAGE(1, "Number of compressed texture formats supported by the driver: %d\n", t);
    if (t == 0)
    {
        MESSAGE(1, "Error: The driver does not support texture compression.\n");
        return false;
    }

    v = new GLint [t];

    glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, v);
    flushGLErrors();

    MESSAGE(2, "Supported compressed texture formats:\n");
    for (int i=0; i<t; i++)
    {
        MESSAGE(2, "format %d: 0x%x ", i, v[i]);
        switch(v[i])
        {
#if defined(GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT: /* 0x83f0 */
            MESSAGE(2, "(GL_COMPRESSED_RGB_S3TC_DXT1_EXT)\n");
            break;
#endif
#if defined(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: /* 0x83f1 */
            MESSAGE(2, "(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)\n");
            break;
#endif
#if defined(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
        case GL_COMPRESSED_RGB_S3TC_DXT3_EXT: /* 0x83f2 */
            MESSAGE(2, "(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)\n");
            break;
#endif
#if defined(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: /* 0x83f3 */
            MESSAGE(2, "(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)\n");
            break;
#endif
#if defined(GL_LUMINANCE_LATC1_EXT)
        case GL_LUMINANCE_LATC1_EXT: /* 0x8c70 */
            MESSAGE(2, "(GL_LUMINANCE_LATC1_EXT)\n");
            break;
#endif
#if defined(GL_SIGNED_LUMINANCE_LATC1_EXT)
        case GL_SIGNED_LUMINANCE_LATC1_EXT: /* 0x8c71 */
            MESSAGE(2, "(GL_SIGNED_LUMINANCE_LATC1_EXT)\n");
            break;
#endif
#if defined(GL_LUMINANCE_ALPHA_LATC2_EXT)
        case GL_LUMINANCE_ALPHA_LATC2_EXT: /* 0x8c72 */
            MESSAGE(2, "(GL_LUMINANCE_ALPHA_LATC2_EXT)\n");
            break;
#endif
#if defined(GL_SIGNED_LUMINANCE_ALPHA_LATC2_EXT)
        case GL_SIGNED_LUMINANCE_ALPHA_LATC2_EXT: /* 0x8c73 */
            MESSAGE(2, "(GL_SIGNED_LUMINANCE_ALPHA_LATC1_EXT)\n");
            break;
#endif
#if defined(COMPRESSED_SRGB_S3TC_DXT1_EXT)
        case COMPRESSED_SRGB_S3TC_DXT1_EXT: /* 0x8c4c */
            MESSAGE(2, "(COMPRESSED_SRGB_S3TC_DXT1_EXT\n");
            break;
#endif
#if defined(COMPRESSED_SRGB_S3TC_DXT1_EXT)
        case COMPRESSED_SRGB_S3TC_DXT1_EXT: /* 0x8c4d */
            MESSAGE(2, "(COMPRESSED_SRGB_S3TC_DXT1_EXT)\n");
            break;
#endif
#if defined(COMPRESSED_SRGB_S3TC_DXT1_EXT)
        case COMPRESSED_SRGB_S3TC_DXT1_EXT: /* 0x8c4e */
            MESSAGE(2, "(COMPRESSED_SRGB_S3TC_DXT1_EXT)\n");
            break;
#endif
#if defined(COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT)
        case COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT: /* 0x8c4f */
            MESSAGE(2, "(COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT)\n");
            break;
#endif
#if defined(GL_ETC1_RGB8_OES)
        case GL_ETC1_RGB8_OES: /* 0x8d64 */
            etc1_supported = true;
            MESSAGE(2, "(GL_ETC1_RGB8_OES)\n");
            break;
#endif
        default:
            MESSAGE(2, "(UNKNOWN FORMAT)\n");
            break;
        }
    }
    delete v;
    return etc1_supported;
}
