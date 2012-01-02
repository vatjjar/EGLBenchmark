
/*
 * b05_RGBTextureTest - benchmark #5, RGB Texture test:
 * ----------------------------------------------------
 * This test tests RGB based texturing on a quadrilateral shape rendering
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "b05_RGBTextureTest.h"

#include "DebugLog.h"
#include "GLWrapper.h"

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
    ss = new SimpleShader();
    if (false == ss->fromFiles(vertex_src, fragment_src))
    {
        DebugLog::Instance()->MESSAGE(2, "Shader program object creation failed\n");
        return false;
    }
#if 0
    shaderProgram = createShaderProgram(vertex_src, fragment_src);
    if (shaderProgram == 0)
    {
        DebugLog::Instance()->MESSAGE(1, "Error: Shader program creation failed\n");
        return false;
    }
#endif
    GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 0, "a_Position");
    GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 1, "a_Texcoord");
    ss->linkProgram();
//    linkShaderProgram(shaderProgram);

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
    GLWrapper::Instance()->GLVIEWPORT(0, 0, w_width, w_height);
    GLWrapper::Instance()->GLCLEAR(GL_COLOR_BUFFER_BIT);
//  GLWrapper::Instance()->GLUSEPROGRAM(shaderProgram);

    GLWrapper::Instance()->GLACTIVETEXTURE(GL_TEXTURE0);
    st->bind();
    GLWrapper::Instance()->GLUNIFORM1I(texturesampler, 0);

    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(0);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(1, 2, GL_FLOAT, GL_FALSE, 0, vTexcoord);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(1);
    GLWrapper::Instance()->GLDRAWARRAYS(GL_TRIANGLES, 0, 6);

    GLWrapper::Instance()->EGLSWAPBUFFERS(egl_display, egl_surface);
}


/*
 * renderSingleFrame()
 */
bool b05_RGBTextureTest::renderSingleFrame(float deltatime)
{
    Render();
    return true;
}

/*
 * getRenderStatistics()
 */
bool b05_RGBTextureTest::getRenderStatistics(RENDER_STATISTICS *rs)
{
    return true;
}
