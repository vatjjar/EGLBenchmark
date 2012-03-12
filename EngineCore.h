
/* EngineCore
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef EngineCore_H
#define EngineCore_H

#include <iostream>

#include  <sys/time.h>
#include  <string.h>
#include  <stdio.h>

#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>

#include  <GLES2/gl2.h>
#include  <GLES2/gl2ext.h>
#include  <EGL/egl.h>

#include "EGLX11Display.h"
#include "EGLX11Benchmark.h"
#include "GLWrapper.h"

/******************************************************************************
 * EngineCore: constants
 */

/******************************************************************************
 * typefes:
 */

/******************************************************************************
 * Class definitions
 */

class EngineCore
{
public:
    EngineCore();
    ~EngineCore();

    // Public API:
    int initCore(void);
    int destroyCore(void);
    int setVerbosityLevel(unsigned int level);

    // EGL display handling
    int createEGLDisplay(unsigned int width, unsigned int height, bool fullscreen);
    int destroyEGLDisplay();

    // Benchmark handling
    int initBenchmark(unsigned int id);
    int runBenchmark(float fpslimit, unsigned int framelimit);
    int runBenchmarkSingleFrame(float deltatime);
    int destroyBenchmark(void);
    int getStatistics(float *ptr);
    int resetStatistics(void);

    // Timer handling
    void resetTimer(void);
    float getTimeSinceTimerReset(void);
    float getTimeSinceLastFrame(void);

protected:
    EGLX11Display *display;
    EGLX11Benchmark *benchmark;

    // Timer API related variables
    struct timeval t_start, t_now, t_lastframe;
    struct timezone tz;

private:
};

#endif // EngineCore_H
