
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

#include "DebugLog.h"

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
 * false must be returned to indicate core benchmark not to continue execution. DebugLog::Instance()->MESSAGE()
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
        DebugLog::Instance()->MESSAGE(1, "Error: Shader program creation failed\n");
        return false;
    }
    glwrap->GLBINDATTRIBLOCATION(shaderProgram, 0, "a_Position");
    glwrap->GLBINDATTRIBLOCATION(shaderProgram, 1, "a_Texcoord");
    linkShaderProgram(shaderProgram);

    texturesampler = glwrap->GLGETUNIFORMLOCATION(shaderProgram, "s_texture");

    /*
     * Texture loading for the test case:
     */
    textureID = loadRGBTexturefromPNG(texturefilename);
    if (textureID == 0)
    {
        DebugLog::Instance()->MESSAGE(1, "Error: Loading of texturefile '%s' failed.\n", texturefilename);
        return false;
    }


    // Try to load simple mesh from disk
    sm = new SimpleMesh();
    if (false == sm->fromFiles("Plane"))
    {
        DebugLog::Instance()->MESSAGE(1, "Unable to load mesh from file Plane\n");
        return false;
    }


    glwrap->GLCLEARCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
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
    glwrap->GLVIEWPORT(0, 0, w_width, w_height);
    glwrap->GLCLEAR(GL_COLOR_BUFFER_BIT);
    glwrap->GLUSEPROGRAM(shaderProgram);

    glwrap->GLACTIVETEXTURE(GL_TEXTURE0);
    glwrap->GLBINDTEXTURE(GL_TEXTURE_2D, textureID);
    glwrap->GLUNIFORM1I(texturesampler, 0);

#if 0
    glwrap->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glwrap->GLENABLEVERTEXATTRIBARRAY(0);
    glwrap->GLVERTEXATTRIBPOINTER(1, 2, GL_FLOAT, GL_FALSE, 0, vTexcoord);
    glwrap->GLENABLEVERTEXATTRIBARRAY(1);
    glwrap->GLDRAWARRAYS(GL_TRIANGLES, 0, 6);
#else
    sm->renderAsIndexedElements_VBO();
#endif

    glwrap->EGLSWAPBUFFERS(egl_display, egl_surface);
}


/*
 * renderSingleFrame()
 */
bool b05_RGBTextureTest::renderSingleFrame(float deltatime)
{
    Render();
    return true;
}

