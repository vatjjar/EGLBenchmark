
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
   EGL_NONE
};

const EGLint ctxattr[] = {
   EGL_CONTEXT_CLIENT_VERSION, 2,
   EGL_NONE
};

typedef struct _render_statistics {
    unsigned int r_vertices;
    unsigned int r_normals;
    unsigned int r_texcoords;
    unsigned int r_faces;
    unsigned int r_batches;
} RENDER_STATISTICS;

// Class definition

class EGLX11Benchmark
{
public:
    EGLX11Benchmark();
    ~EGLX11Benchmark();

public:
    virtual bool initBenchmark(unsigned int width, unsigned int height, bool fullscreen) = 0;
    virtual bool destroyBenchmark(void) = 0;
    virtual bool renderSingleFrame(float deltatime) = 0;
    virtual bool getRenderStatistics(RENDER_STATISTICS *rs) = 0;
    virtual bool keyHandler(char text) {
        if (text == 033) return true;  /* ESC */
        return false;
    }

    // Wraps for GL error fetching
    void flushGLErrors(void);
    void flushEGLErrors(void);
    unsigned int getGLErrors(void);
    unsigned int getEGLErrors(void);

    // verbosity
    void setVerbosityLevel(unsigned int level);

    // Methods for timers and user break
    void resetTimer(void);
    float getTimeSinceTimerReset(void);
    float getTimeSinceLastFrame(void);
    bool userInterrupt(void);

    // Getters for benchmark naming info
    const char * getName(void);
    const char * getDescription(void);

protected:
    // Benchmark class helper methods for EGL context handling
    bool createEGLDisplay(int width, int height, bool fullscreen);
    void destroyEGLDisplay(void);

    void setName(const char *);
    void setDescription(const char *);

    // Benchmark case name and description
    const char * name;
    const char * description;

    // Logical render window properties
    unsigned int w_width;
    unsigned int w_height;
    bool w_fullscreen;

    // X11 related
    Window      win;
    Display    *x_display;
    EGLDisplay  egl_display;
    EGLSurface  egl_surface;

    // Timer related
    struct timeval t_start, t_now, t_lastframe;
    struct timezone tz;

private:
    // Variables EGL
    EGLContext  egl_context;
};

#endif // EGLX11DISPLAY_H

