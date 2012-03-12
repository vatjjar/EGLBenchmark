
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef EGLX11DISPLAY_H
#define EGLX11DISPLAY_H

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

#include "GLWrapper.h"

const EGLint attr[] = {       // some attributes to set up our egl-interface
   EGL_BUFFER_SIZE,     16,
   EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
   //EGL_ALPHA_SIZE,      8,
   EGL_DEPTH_SIZE,      8,
   EGL_NONE
};

const EGLint ctxattr[] = {
   EGL_CONTEXT_CLIENT_VERSION, 2,
   EGL_NONE
};

// Class definition

class EGLX11Display
{
public:
    EGLX11Display();
    ~EGLX11Display();

public:
    // Benchmark class helper methods for EGL context handling
    int createEGLDisplay(int width, int height, bool fullscreen);
    int destroyEGLDisplay(void);

    Display *getXDisplay(void);
    EGLDisplay getEGLDisplay(void);
    EGLSurface getEGLSurface(void);
    unsigned int getDisplayWidth(void);
    unsigned int getDisplayHeight(void);

private:
    // Variables EGL
    EGLContext  egl_context;

    // Logical render window properties
    unsigned int w_width;
    unsigned int w_height;
    bool w_fullscreen;

    // X11 related
    Window      win;
    Display    *x_display;
    EGLDisplay  egl_display;
    EGLSurface  egl_surface;
};

#endif // EGLX11DISPLAY_H
