/*
 * __API.cpp: ctypes API for python logic code
 * -----------------------------------------------------
 * This is the API for external python code to invoke internal Benchmark API
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "__API.h"

#include "EngineCore.h"

/******************************************************************************
 * Static core class
 */

static EngineCore *core = NULL;

/******************************************************************************
 * General subsystem init
 */

int initCore()
{
    if (core != NULL) return 0;
    core = new EngineCore();
    core->initCore();
    return 0;
}

int destroyCore()
{
    if (core == NULL) return 0;
    core->destroyCore();
    delete core;
    core = NULL;
    return 0;
}

int setVerbosityLevel(unsigned int level)
{
    if (core == NULL) return -1;
    return core->setVerbosityLevel(level);
}

/******************************************************************************
 * Display I/O methods
 */

int createEGLDisplay(unsigned int width, unsigned int height, bool fullscreen)
{
    if (core == NULL) return -1;
    return core->createEGLDisplay(width, height, fullscreen);
}

int destroyEGLDisplay()
{
    if (core == NULL) return -1;
    return core->destroyEGLDisplay();
}

/******************************************************************************
 * Benchmark methods
 */

int initBenchmark(unsigned int id)
{
    if (core == NULL) return -1;
    return core->initBenchmark(id);
}

int runBenchmark(float fpslimit, unsigned int framelimit)
{
    if (core == NULL) return -1;
    return core->runBenchmark(fpslimit, framelimit);
}

int runBenchmarkSingleFrame(float deltatime)
{
    if (core == NULL) return -1;
    return core->runBenchmarkSingleFrame(deltatime);
}

int destroyBenchmark(void)
{
    if (core == NULL) return -1;
    return core->destroyBenchmark();
}

int getStatistics(float *params)
{
    if (core == NULL) return -1;
    return core->getStatistics(params);
}

int resetStatistics(void)
{
    if (core == NULL) return -1;
    return core->resetStatistics();
}
