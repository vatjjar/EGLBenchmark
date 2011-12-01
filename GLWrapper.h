
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef GLWrapper_H
#define GLWrapper_H

#include  <GLES2/gl2.h>
#include  <GLES2/gl2ext.h>
#include  <EGL/egl.h>

class GLWrapper {
public:
    GLWrapper();
    ~GLWrapper();

    void flushGLErrors(void);
    void flushEGLErrors(void);
    unsigned int getGLErrors(void);
    unsigned int getEGLErrors(void);

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
    GLint  GLGETATTRIBLOCATION(GLuint program, const GLchar *name);
    GLint  GLGETUNIFORMLOCATION(GLuint program, const GLchar *name);
    void   GLACTIVETEXTURE(GLenum texture);
    void   GLUNIFORM1I(GLint location, GLint x);
    void   GLUNIFORM1F(GLint location, GLfloat x);
    void   GLUNIFORM4F(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void   GLCOMPRESSEDTEXIMAGE2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint height, GLint border,
                                       GLsizei size, const GLvoid *data);
    void   GLDRAWELEMENTS(GLenum mode, GLsizei count, GLenum type, const GLvoid *data);

    // EGL Wrappers:
    void   EGLSWAPBUFFERS(EGLDisplay, EGLSurface egl_surface);

protected:

private:
    // Error counters
    unsigned int GLerrors;
    unsigned int EGLerrors;
};

#endif // GLWrapper_H
