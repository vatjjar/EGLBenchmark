
/*
 * b05_RGBTextureTest - benchmark #1, EGL context creation:
 * -----------------------------------------------------
 * This test case initializes native window, creates render surfaces and associated EGL context
 * to hold all of this information. No other actions are taken. This is the simplest test for EGL
 * framework.
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "b05_RGBTextureTest.h"

/*
 * Constructor and destructor are dummy ones. Only descriptions are set, and other activities are
 * done when calling the virtual benchmark API
 */

b05_RGBTextureTest::b05_RGBTextureTest()
{
    setName("RGB texture mapping test");
    setDescription("This test tests simple texture mapping on a quad using RGB based texture");
}

b05_RGBTextureTest::~b05_RGBTextureTest()
{
}

/*
 * initBenchmark() shall initialize all required resources for this test case. If initialization fails,
 * false must be returned to indicate core benchmark not to continue execution. Parent class MESSAGE()
 * method can be used to output information about the initialization
 */
bool b05_RGBTextureTest::initBenchmark(unsigned int width, unsigned int height, bool fullscreen)
{
    const char *texturefilename = "./resources/pngRGB.png";
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


    /*
     * Display and context init
     */
    if (false == createEGLDisplay(width, height, fullscreen))
    {
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
    textureID = loadRGBTexturefromPNG(texturefilename);
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
bool b05_RGBTextureTest::destroyBenchmark(void)
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

void b05_RGBTextureTest::Render(void)
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
bool b05_RGBTextureTest::runBenchmark(float duration)
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
bool b05_RGBTextureTest::displayResult(void)
{
    MESSAGE_1P(1, "Total rendering time %f\n", totaltime);
    MESSAGE_1P(1, "Total rendered frames %d\n", renderedFrames);
    MESSAGE_1P(1, "Frames per second %f\n", renderedFrames/totaltime);
    return true;
}
