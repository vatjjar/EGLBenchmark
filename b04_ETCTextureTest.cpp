
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
    GLint f[10];

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
        switch(f[i])
        {
        default:
            MESSAGE(2, "format %d: ", i);
            MESSAGE(2, "0x%x ", f[i]);
            outputMessage(2, "(UNKNOWN FORMAT)\n");
            break;
        }
    }
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

/*
 * runBenchmark()
 */
bool b04_ETCTextureTest::runBenchmark(float duration)
{
    return true;
}

/*
 * displayResult()
 */
bool b04_ETCTextureTest::displayResult(void)
{
    return true;
}
