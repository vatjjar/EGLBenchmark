
/*
 * b08_Scenegraph - benchmark #8, Simple Scenegraph rendering test
 * ---------------------------------------------------------------
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "b08_Scenegraph.h"

#include <math.h>

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
bool b08_Scenegraph::initBenchmark(void)
{
    ssg = new SimpleScenegraph();
    if (false == ssg->initScenegraph(display->getDisplayWidth(), display->getDisplayHeight()))
    {
        DebugLog::Instance()->MESSAGE(2, "Scenegraph initialization failed!");
        return false;
    }
    if (false == ssg->fromFile("./scenegraph.txt"))
    {
        DebugLog::Instance()->MESSAGE(2, "Scenegraph loading failed from scenegraph.txt\n");
        return false;
    }
    ssg->setCameraLocation(0.0f, 0.5f, 0.0f);

    // If we have errors in GL pipe, then abort.
    if (GLWrapper::Instance()->getGLErrors() > 0) return false;

    return true;
}

bool b08_Scenegraph::keyHandler(char text)
{
    switch (text)
    {
    case 033: /* ESC */
        return true;
    case 'w':
        ssg->setCameraDelta(0.0f, 0.0f, 0.1f);
        break;
    case 's':
        ssg->setCameraDelta(0.0f, 0.0f, -0.1f);
        break;
    case 'a':
        ssg->setCameraDelta(0.1f, 0.0f, 0.0f);
        break;
    case 'd':
        ssg->setCameraDelta(-0.1f, 0.0f, 0.0f);
        break;
    case 'r':
        ssg->setCameraDelta(0.0f, -0.1f, 0.0f);
        break;
    case 'f':
        ssg->setCameraDelta(0.0f, 0.1f, 0.0f);
        break;
    }
    return false;
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b08_Scenegraph::destroyBenchmark(void)
{
    return true;
}

/*
 * renderSingleFrame()
 */
bool b08_Scenegraph::renderSingleFrame(float timedelta)
{
    ssg->render();

    // get the rendered buffer to the screen
    GLWrapper::Instance()->EGLSWAPBUFFERS(display->getEGLDisplay(), display->getEGLSurface());

    ssg->setCameraDelta(0.0f, 0.0f, -0.1f*timedelta);
    return true;
}


/*
 * getRenderStatistics()
 */
bool b08_Scenegraph::getRenderStatistics(RENDER_STATISTICS *rs)
{
    return true;
}

