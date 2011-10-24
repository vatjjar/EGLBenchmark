
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
#include  <EGL/egl.h>

const EGLint attr[] = {       // some attributes to set up our egl-interface
   EGL_BUFFER_SIZE,     16,
   EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
   EGL_NONE
};

const EGLint ctxattr[] = {
   EGL_CONTEXT_CLIENT_VERSION, 2,
   EGL_NONE
};

#define MESSAGE(level, str, param) \
   { \
       char buf[128]; \
       sprintf(buf, str, param); \
       outputMessage(level, (const char*)&buf); \
   }

// Class definition

class EGLX11Benchmark
{
public:
    EGLX11Benchmark();
    ~EGLX11Benchmark();

public:
    virtual bool initBenchmark(unsigned int width, unsigned int height, bool fullscreen) = 0;
    virtual bool destroyBenchmark(void) = 0;
    virtual bool runBenchmark(float duration) = 0;
    virtual bool displayResult(void) = 0;

    // Methods for messages & debug output
    void outputMessage(int level, const char *message);
    void setVerbosityLevel(unsigned int level);
    void flushGLErrors(void);
    void flushEGLErrors(void);
    unsigned int getGLErrors(void);
    unsigned int getEGLErrors(void);

    // Methods for timers and user break
    void resetTimer(void);
    float getTime(void);
    bool userInterrupt(void);

    // Helper methods for shader handling
    GLuint loadShader(const char *shader_source, GLenum type);
    void printShaderInfo(GLuint shader);

    // Getters for benchmark naming info
    const char * getName(void);
    const char * getDescription(void);

protected:
    // GL wrappers, for better handling of errors et al
    void GLCLEARCOLOR(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void GLLINKPROGRAM(GLuint program);
    void GLUSEPROGRAM(GLuint program);

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
    struct timeval t1, t2;
    struct timezone tz;

private:
    // Variables EGL
    EGLContext  egl_context;
    // Other
    int verbosity;
    unsigned int GLerrors;
    unsigned int EGLerrors;

};

#endif // EGLX11DISPLAY_H

