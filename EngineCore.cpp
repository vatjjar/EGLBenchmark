
/* EngineCore
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "EngineCore.h"
#include "EGLX11Display.h"
#include "DebugLog.h"

#include "b01_ContextInit.h"
#include "b02_SimpleGLShading.h"
#include "b03_SimpleTriangle.h"
#include "b04_ETCTextureTest.h"
#include "b05_RGBTextureTest.h"
#include "b06_VBOElementsRGB.h"
#include "b07_PointCloud.h"
#include "b08_Scenegraph.h"
#include "b09_Terrain.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

/******************************************************************************
 * Constructor and destructor
 */

EngineCore::EngineCore() :
    display(NULL),
    benchmark(NULL)
{
    setVerbosityLevel(1); // Default verbosity
}

EngineCore::~EngineCore()
{

}

/******************************************************************************
 * Core
 */

int EngineCore::initCore(void)
{
    return 0;
}

int EngineCore::destroyCore(void)
{
    return destroyEGLDisplay();
}

int EngineCore::setVerbosityLevel(unsigned int level)
{
    DebugLog::Instance()->setVerbosityLevel(level);
    return 0;
}

/******************************************************************************
 * Display creation API
 */

int EngineCore::createEGLDisplay(unsigned int width, unsigned int height, bool fullscreen)
{
    if (display != NULL)
    {
        destroyEGLDisplay();
    }
    display = new EGLX11Display();
    return display->createEGLDisplay(width, height, fullscreen);
}

int EngineCore::destroyEGLDisplay()
{
    int rc;
    if (display == NULL) return -1;
    rc = display->destroyEGLDisplay();
    delete display;
    display = NULL;
    return rc;
}

/******************************************************************************
 * Benchmark methods:
 */

int EngineCore::initBenchmark(unsigned int id)
{
    destroyBenchmark(); // Destroy possible previous benchmark entity
    switch(id)
    {
    case TEST_CONTEXTINIT:
        benchmark = dynamic_cast<EGLX11Benchmark*>(new b01_ContextInit());
        break;
    case TEST_SIMPLESHADER:
        benchmark = dynamic_cast<EGLX11Benchmark*>(new b02_SimpleGLShading());
        break;
    case TEST_SIMPLETRIANGLE:
        benchmark = dynamic_cast<EGLX11Benchmark*>(new b03_SimpleTriangle());
        break;
    case TEST_ETCTEXTURE:
        benchmark = dynamic_cast<EGLX11Benchmark*>(new b04_ETCTextureTest());
        break;
    case TEST_RGBTEXTURE:
        benchmark = dynamic_cast<EGLX11Benchmark*>(new b05_RGBTextureTest());
        break;
    case TEST_VBOELEMENTSRGB:
        benchmark = dynamic_cast<EGLX11Benchmark*>(new b06_VBOElementsRGB());
        break;
    case TEST_POINTCLOUD:
        benchmark = dynamic_cast<EGLX11Benchmark*>(new b07_PointCloud());
        break;
    case TEST_SCENEGRAPH:
        benchmark = dynamic_cast<EGLX11Benchmark*>(new b08_Scenegraph());
        break;
    case TEST_TERRAIN:
        benchmark = dynamic_cast<EGLX11Benchmark*>(new b09_Terrain());
        break;
    default:
        DebugLog::Instance()->MESSAGE(2, "Warning, trying to init testcase ID=%d which does not exist!\n", id);
        return -1;
    }
    if (benchmark == NULL) return -1;

    benchmark->setEGLDisplay(display);
    if (false == benchmark->initBenchmark()) return -1;
    return 0;
}

int EngineCore::runBenchmark(float fpslimit, unsigned int framelimit)
{
    float totaltime, deltatime;
    unsigned int renderedFrames;
    if (benchmark == NULL) return -1;

    DebugLog::Instance()->MESSAGE(5, "runbenchmark(%f, %d)\n", fpslimit, framelimit);

    // Timer and variables
    resetTimer();
    totaltime = 0.0f;
    deltatime = 0.0f;
    renderedFrames = 0;

    while (renderedFrames < framelimit)
    {
        getTimeSinceLastFrame();
        if (false == runBenchmarkSingleFrame(deltatime))
        {
            DebugLog::Instance()->MESSAGE(1, "Error during framerender. Abort!\n");
            return -1;
        }

        // Deltatime is time taken by the render process. It does not take fpslimit or
        // othes issues into account.
        deltatime = getTimeSinceLastFrame();
        //frametimes[renderedFrames] = deltatime;
        renderedFrames++;

        // FPS limiter
        if (fpslimit > 0.0f)
        {
            float frametime;
            frametime = 1.0f/fpslimit;
            if (deltatime < frametime)
            {
                useconds_t sl = (useconds_t)((frametime-deltatime)*1000000);
                usleep(sl);
                deltatime = frametime;
            }
        }
        totaltime += deltatime;

        if (benchmark->userInterrupt() == true)
        {
            DebugLog::Instance()->MESSAGE(1, "User interrupt!\n");
            break;
        }
    }
    return 0;
}

int EngineCore::runBenchmarkSingleFrame(float deltatime)
{
    if (benchmark == NULL) return -1;
    return benchmark->renderSingleFrame(deltatime);
}

int EngineCore::destroyBenchmark(void)
{
    if (benchmark == NULL) return -1;
    benchmark->destroyBenchmark();
    delete benchmark;
    benchmark = NULL;
    return 0;
}

int EngineCore::getStatistics(float *ptr)
{
    if (benchmark == NULL) return -1;
    return 0;
}

int EngineCore::resetStatistics(void)
{
    if (benchmark == NULL) return -1;
    return 0;
}

/******************************************************************************
 * Timer methods:
 */

void EngineCore::resetTimer(void)
{
    gettimeofday ( &t_start , &tz );
    t_now = t_lastframe = t_start;
}

float EngineCore::getTimeSinceTimerReset(void)
{
    gettimeofday(&t_now, &tz);
    return (float)(t_now.tv_sec - t_start.tv_sec + (t_now.tv_usec - t_start.tv_usec) * 1e-6);
}

float EngineCore::getTimeSinceLastFrame(void)
{
    float delta;
    gettimeofday(&t_now, &tz);
    delta = (float)(t_now.tv_sec - t_lastframe.tv_sec + (t_now.tv_usec - t_lastframe.tv_usec) * 1e-6);
    t_lastframe = t_now;
    return delta;
}
