
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
    setName("ETC texture compression test");
    setDescription("This test tests texture compression parts of GLES2 and executes a case using ETC texture format");
    etc1_supported = false;
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
    GLint t;
    GLint f[32];
    const char *texturefilename = "./resources/elina.pkm";
#if 0
    const char vertex_src[] =
       "attribute vec4 a_Position;   \n"
       "attribute vec2 a_TexCoord;   \n"
       "varying vec2 v_TexCoord;     \n"
       "void main()                  \n"
       "{                            \n"
       "   gl_Position = a_Position; \n"
       "   v_TexCoord = a_TexCoord;  \n"
       "}                            \n";

    const char fragment_src[] =
       "precision mediump float;                     \n"
       "varying vec2 v_TexCoord;                     \n"
       "uniform sampler2D s_texture;                 \n"
       "void main()                                  \n"
       "{                                            \n"
       "  gl_FragColor = texture2D(s_texture, v_TexCoord);\n"
       "}                                            \n";
#else
    const char vertex_src[] =
    "attribute vec4 vPosition;    \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = vPosition;  \n"
    "}                            \n";

 const char fragment_src[] =
    "precision mediump float;\n"\
    "void main()                                  \n"
    "{                                            \n"
    "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
    "}                                            \n";
#endif

    // Display and context init

    if (false == createEGLDisplay(width, height, fullscreen))
    {
        return false;
    }

    // First we query the list of supported compressed texture formats
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &t);
    flushGLErrors();
    MESSAGE_1P(1, "Number of compressed texture formats supported by the driver: %d\n", t);

    glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, (GLint*)&f);
    flushGLErrors();
    MESSAGE(2, "Supported compressed texture formats:\n");
    for (int i=0; i<t; i++)
    {
        MESSAGE_1P(2, "format %d: ", i);
        MESSAGE_1P(2, "0x%x ", f[i]);
        switch(f[i])
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

    if (etc1_supported == false)
    {
        MESSAGE(2, "No ETC1 support detected in drivers\n");
        //return false;
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
    linkShaderProgram(shaderProgram);

    /*
     * Texture loading for the test case:
     */
    textureID = loadETCTextureFromFile(texturefilename);
    if (textureID == 0)
    {
        MESSAGE_1P(1, "Error: Loading of texturefile '%s' failed.\n", texturefilename);
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


void b04_ETCTextureTest::Render(void)
{
    GLfloat vVertices[] = {  -0.5f, -0.5f, 0.0f,
                             -0.5f,  0.5f, 0.0f,
                              0.5f,  0.5f, 0.0f,
                              0.5f, -0.5f, 0.0f };

    GLVIEWPORT(0, 0, w_width, w_height);
    GLCLEAR(GL_COLOR_BUFFER_BIT);
    GLUSEPROGRAM(shaderProgram);
    GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    GLENABLEVERTEXATTRIBARRAY(0);
    GLDRAWARRAYS(GL_TRIANGLES, 0, 3);

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
    return true;
}
