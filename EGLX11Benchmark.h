
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

#include "EGLX11Benchmark_Debug.h"

const EGLint attr[] = {       // some attributes to set up our egl-interface
   EGL_BUFFER_SIZE,     16,
   EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
   EGL_NONE
};

const EGLint ctxattr[] = {
   EGL_CONTEXT_CLIENT_VERSION, 2,
   EGL_NONE
};

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

    // Getters for benchmark naming info
    const char * getName(void);
    const char * getDescription(void);

protected:
    // GL wrappers, for better handling of errors et al
    void   GLATTACHSHADER(GLuint shaderProgram, GLuint shader);
    void   GLBINDATTRIBLOCATION(GLuint shaderProgram, GLuint index, const GLchar *name);
    void   GLCLEARCOLOR(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    GLuint GLCREATEPROGRAM(void);
    void   GLLINKPROGRAM(GLuint program);
    void   GLUSEPROGRAM(GLuint program);
    void   GLVIEWPORT(GLint x, GLint y, GLsizei width, GLsizei height);
    void   GLCLEAR(GLbitfield mask);
    void   GLVERTEXATTRIBPOINTER(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *data);
    void   GLENABLEVERTEXATTRIBARRAY(GLuint index);
    void   GLDRAWARRAYS(GLenum mode, GLint first, GLsizei size);
    void   GLGENTEXTURES(GLsizei size, GLuint *ptr);
    void   GLBINDTEXTURE(GLenum target, GLuint id);
    void   GLPIXELSTOREI(GLenum type, GLint align);
    void   GLTEXIMAGE2D(GLenum target, GLint level, GLint inernalformat, GLsizei width,
                                       GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
    void   GLTEXPARAMETERI(GLenum target, GLenum pname, GLint param);
    GLint  GLGETUNIFORMLOCATION(GLuint program, const GLchar *name);
    void   GLACTIVETEXTURE(GLenum texture);
    void   GLUNIFORM1I(GLint location, GLint x);

    // EGL Wrappers:
    void   EGLSWAPBUFFERS(EGLDisplay, EGLSurface egl_surface);

    // Benchmark class helper methods for EGL context handling
    bool createEGLDisplay(int width, int height, bool fullscreen);
    void destroyEGLDisplay(void);

    // Helper methods for shader handling
    GLuint createShaderProgram(const char *v_src, const char *f_src);
    GLuint loadShaderProgram(const char *shader_source, GLenum type);
    void linkShaderProgram(GLuint shaderProgram);
    void printShaderInfo(GLuint shader);

    // Helpers for simple file I/O
    unsigned char * readBinaryFile(const char *filename, unsigned int & length);

    // Helpers for texture loading
    GLuint loadETCTextureFromFile(const char *filename);
    GLuint loadRGBTexturefromPNG(const char *filename);

    void setName(const char *);
    void setDescription(const char *);

    void MESSAGE(int level, const char *format, ...);

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

