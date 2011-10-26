
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */


#include "EGLX11Benchmark.h"
#include "EGLX11Benchmark_EGLWrappers.h"


/*
 * EGL Wrappers:
 * ------------
 * The intention of these methods are to easily wrap EGL calls and bundle them with something
 * useful, like debug information and error grabbing. This way seems most suitable for the
 * purpose.
 */

void EGLX11Benchmark::EGLSWAPBUFFERS(EGLDisplay egl_display, EGLSurface egl_surface)
{
    MESSAGE_2P(5, "Attempting a call to eglSwapBuffers(0x%p, 0x%p)\n", egl_display, egl_surface);
    eglSwapBuffers(egl_display, egl_surface);
    flushEGLErrors();
}

