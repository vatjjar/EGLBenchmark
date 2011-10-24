
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
 * false must be returned to indicate core benchmark not to continue execution. Parent class outputMessage()
 * method can be used to output information about the initialization
 */
bool b04_ETCTextureTest::initBenchmark(unsigned int width, unsigned int height, bool fullscreen)
{
    GLint t;
    GLint f[32];
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

    // Display and context init

    if (false == createEGLDisplay(width, height, fullscreen))
    {
        return false;
    }

    // First we query the list of supported compressed texture formats
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &t);
    flushGLErrors();
    MESSAGE(1, "Number of compressed texture formats supported by the driver: %d\n", t);

    glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, (GLint*)&f);
    flushGLErrors();
    outputMessage(2, "Supported compressed texture formats:\n");
    for (int i=0; i<t; i++)
    {
        MESSAGE(2, "format %d: ", i);
        MESSAGE(2, "0x%x ", f[i]);
        switch(f[i])
        {
#if defined(GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT: /* 0x83f0 */
            outputMessage(2, "(GL_COMPRESSED_RGB_S3TC_DXT1_EXT)\n");
            break;
#endif
#if defined(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: /* 0x83f1 */
            outputMessage(2, "(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)\n");
            break;
#endif
#if defined(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
        case GL_COMPRESSED_RGB_S3TC_DXT3_EXT: /* 0x83f2 */
            outputMessage(2, "(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)\n");
            break;
#endif
#if defined(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: /* 0x83f3 */
            outputMessage(2, "(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)\n");
            break;
#endif
#if defined(GL_LUMINANCE_LATC1_EXT)
        case GL_LUMINANCE_LATC1_EXT: /* 0x8c70 */
            outputMessage(2, "(GL_LUMINANCE_LATC1_EXT)\n");
            break;
#endif
#if defined(GL_SIGNED_LUMINANCE_LATC1_EXT)
        case GL_SIGNED_LUMINANCE_LATC1_EXT: /* 0x8c71 */
            outputMessage(2, "(GL_SIGNED_LUMINANCE_LATC1_EXT)\n");
            break;
#endif
#if defined(GL_LUMINANCE_ALPHA_LATC2_EXT)
        case GL_LUMINANCE_ALPHA_LATC2_EXT: /* 0x8c72 */
            outputMessage(2, "(GL_LUMINANCE_ALPHA_LATC2_EXT)\n");
            break;
#endif
#if defined(GL_SIGNED_LUMINANCE_ALPHA_LATC2_EXT)
        case GL_SIGNED_LUMINANCE_ALPHA_LATC2_EXT: /* 0x8c73 */
            outputMessage(2, "(GL_SIGNED_LUMINANCE_ALPHA_LATC1_EXT)\n");
            break;
#endif
#if defined(GL_ETC1_RGB8_OES)
        case GL_ETC1_RGB8_OES: /* 0x8d64 */
            etc1_supported = true;
            outputMessage(2, "(GL_ETC1_RGB8_OES)\n");
            break;
#endif
        default:
            outputMessage(2, "(UNKNOWN FORMAT)\n");
            break;
        }
    }

    if (etc1_supported == false)
    {
        outputMessage(2, "No ETC1 support detected in drivers\n");
        //return false;
    }

    // Then shader init:

    outputMessage(5, "loading shaders...\n");
    vertexShader   = loadShader ( vertex_src , GL_VERTEX_SHADER  );     // load vertex shader
    fragmentShader = loadShader ( fragment_src , GL_FRAGMENT_SHADER );  // load fragment shader
    if (vertexShader == 0 || fragmentShader == 0)
    {
        outputMessage(1, "Error: Shader program loading failed\n");
        return false;
    }

    outputMessage(5, "glCreateProgram()\n");
    shaderProgram  = glCreateProgram ();
    flushGLErrors();
    outputMessage(5, "glAttachShader() - vertex\n");
    glAttachShader ( shaderProgram, vertexShader );
    flushGLErrors();
    outputMessage(5, "glAttachShader() - fragment\n");
    glAttachShader ( shaderProgram, fragmentShader );
    flushGLErrors();

    glBindAttribLocation ( shaderProgram, 0, "a_Position" );
    flushGLErrors();

#if 0
    outputMessage(5, "glLinkProgram()\n");
    glLinkProgram ( shaderProgram );
    flushGLErrors();
    outputMessage(5, "glUseProgram()\n");
    glUseProgram  ( shaderProgram );
    flushGLErrors();
#else
    GLLINKPROGRAM(shaderProgram);
    GLUSEPROGRAM(shaderProgram);
#endif

    GLCLEARCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
    //glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    flushGLErrors();

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

    // Set the viewport
    glViewport ( 0, 0, w_width, w_height);
    flushGLErrors();

    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );
    flushGLErrors();

    // Use the program object
    glUseProgram ( shaderProgram );
    flushGLErrors();

    // Load the vertex data
    glVertexAttribPointer ( 0, 4, GL_FLOAT, GL_FALSE, 0, vVertices );
    flushGLErrors();

    glEnableVertexAttribArray ( 0 );
    flushGLErrors();

    glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );
    flushGLErrors();

    eglSwapBuffers ( egl_display, egl_surface );  // get the rendered buffer to the screen
    flushGLErrors();
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
