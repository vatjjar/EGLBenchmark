
/*
 * b01_ContextInit - benchmark #1, EGL context creation:
 * -----------------------------------------------------
 * This test case initializes native window, creates render surfaces and associated EGL context
 * to hold all of this information. No other actions are taken. This is the simplest test for EGL
 * framework.
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "b01_ContextInit.h"

/*
 * Constructor and destructor are dummy ones. Only descriptions are set, and other activities are
 * done when calling the virtual benchmark API
 */

b01_ContextInit::b01_ContextInit()
{
    setName("EGL Context Init test");
    setDescription("This test executes EGL context initialization test");
}

b01_ContextInit::~b01_ContextInit()
{
}

/*
 * initBenchmark() shall initialize all required resources for this test case. If initialization fails,
 * false must be returned to indicate core benchmark not to continue execution. Parent class outputMessage()
 * method can be used to output information about the initialization
 */
bool b01_ContextInit::initBenchmark(unsigned int width, unsigned int height, bool fullscreen)
{
    w_width = width;
    w_height = height;
    w_fullscreen = fullscreen;
    return true;
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b01_ContextInit::destroyBenchmark(void)
{
    destroyEGLDisplay();
    return true;
}

/*
 * runBenchmark()
 */
bool b01_ContextInit::runBenchmark(float duration)
{
    return createEGLDisplay(w_width, w_height, w_fullscreen);
}

/*
 * displayResult()
 */
bool b01_ContextInit::displayResult(void)
{
    return false;
}
