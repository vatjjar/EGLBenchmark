/*
 * __API.h: ctypes API for python logic code
 * -----------------------------------------------------
 * This is the API for external python code to invoke internal Benchmark API
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef __API_H
#define __API_H

extern "C" {

/******************************************************************************
 * General subsystem init
 */

int initCore();
int destroyCore();
int setVerbosityLevel(unsigned int level);

/******************************************************************************
 * Display I/O methods
 */

int createEGLDisplay(unsigned int width, unsigned int height, bool fullscreen);
int destroyEGLDisplay();

/******************************************************************************
 * Benchmark methods
 */

int initBenchmark(unsigned int id);
int runBenchmark(float fpslimit, unsigned int framelimit);
int runBenchmarkSingleFrame(float deltatime);
int destroyBenchmark(void);
int getStatistics(float *params);
int resetStatistics(void);

} // extern "C"

#endif // __API_H
