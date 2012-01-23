
/*
 * b08_Scenegraph - benchmark #8, Simple Scenegraph rendering test
 * ---------------------------------------------------------------
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "b08_Scenegraph.h"


/*************************************************************************************************
 * Constructor and destructor are dummy ones. Only descriptions are set, and other activities are
 * done when calling the virtual benchmark API
 */

b08_Scenegraph::b08_Scenegraph()
{
    setName("Simple Scenegraph rendering test");
    setDescription("This test reads and external scenegraph definiton and renders it");
}

b08_Scenegraph::~b08_Scenegraph()
{
}

/*
 * initBenchmark() shall initialize all required resources for this test case. If initialization fails,
 * false must be returned to indicate core benchmark not to continue execution. Parent class outputMessage()
 * method can be used to output information about the initialization
 */
bool b08_Scenegraph::initBenchmark(unsigned int width, unsigned int height, bool fullscreen)
{
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

    if (false == createEGLDisplay(width, height, fullscreen))
    {
        return false;
    }

    ss = new SimpleShader();
    if (false == ss->fromFiles(vertex_src, fragment_src))
    {
        DebugLog::Instance()->MESSAGE(2, "Shader program object creation failed\n");
        return false;
    }
    GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 0, "vPosition");
    ss->linkProgram();

    GLWrapper::Instance()->GLCLEARCOLOR(0, 0, 0, 0);
    GLWrapper::Instance()->GLVIEWPORT(0, 0, w_width, w_height);
    return true;
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b08_Scenegraph::destroyBenchmark(void)
{
    destroyEGLDisplay();
    return true;
}

/*
 * renderSingleFrame()
 */
bool b08_Scenegraph::renderSingleFrame(float timedelta)
{
    GLWrapper::Instance()->GLVIEWPORT(0, 0, w_width, w_height);
    GLWrapper::Instance()->GLCLEAR(GL_COLOR_BUFFER_BIT);
    ss->bindProgram();
    GLWrapper::Instance()->EGLSWAPBUFFERS ( egl_display, egl_surface );  // get the rendered buffer to the screen
    return true;
}


/*
 * getRenderStatistics()
 */
bool b08_Scenegraph::getRenderStatistics(RENDER_STATISTICS *rs)
{
    return true;
}
