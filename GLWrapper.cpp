
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */


#include "GLWrapper.h"

/*
 * GL Wrappers:
 * ------------
 * The intention of these methods are to easily wrap GL calls and bundle them with something
 * useful, like debug information and error grabbing. This way seems most suitable for the
 * purpose.
 */

void GLWrapper::GLATTACHSHADER(GLuint shaderProgram, GLuint shader)
{
    MESSAGE(4, "GL call: glAttachShader(%d, %d)\n", shaderProgram, shader);
    glAttachShader(shaderProgram, shader);
    flushGLErrors();
}

void GLWrapper::GLBINDATTRIBLOCATION(GLuint shaderProgram, GLuint index, const GLchar *name)
{
    MESSAGE(4, "GL call: glBindAttribLocation(%d, %d, '%s')\n", shaderProgram, index, name);
    glBindAttribLocation(shaderProgram, index, name);
    flushGLErrors();
}
void GLWrapper::GLCLEARCOLOR(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
    MESSAGE(4, "GL call: glClearColor(%f, %f, %f, %f)\n", r, g, b, a);
    glClearColor(r, g, b, a);
    flushGLErrors();
}

GLuint GLWrapper::GLCREATEPROGRAM(void)
{
    GLuint program;
    program = glCreateProgram();
    MESSAGE(4, "GL call: glCreateProgram() = %d\n", program);
    flushGLErrors();
    return program;
}

void GLWrapper::GLLINKPROGRAM(GLuint program)
{
    MESSAGE(4, "GL call: glLinkProgram(%d)\n", program);
    glLinkProgram(program);
    flushGLErrors();
}

void GLWrapper::GLUSEPROGRAM(GLuint program)
{
    MESSAGE(5, "GL call: glUseProgram(%d)\n", program);
    glUseProgram(program);
    flushGLErrors();
}

void GLWrapper::GLVIEWPORT(GLint x, GLint y, GLsizei width, GLsizei height)
{
    MESSAGE(5, "GL call: glViewPort(%d, %d, %d, %d)\n", x, y, width, height);
    glViewport(x, y, width, height);
    flushGLErrors();
}

void GLWrapper::GLCLEAR(GLbitfield mask)
{
    MESSAGE(5, "GL call: glClear(0x%x)\n", mask);
    glClear(mask);
    flushGLErrors();
}

void GLWrapper::GLVERTEXATTRIBPOINTER(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *data)
{
    MESSAGE(5, "GL call: glVertexAttribPointer(%d, %d, %d, %d, %d, %p)\n",
               index, size, type, normalized, stride, data);
    glVertexAttribPointer(index, size, type, normalized, stride, data);
    flushGLErrors();
}

void GLWrapper::GLENABLEVERTEXATTRIBARRAY(GLuint index)
{
    MESSAGE(5, "GL call: glEnableVertexAttribPointer(%d)\n", index);
    glEnableVertexAttribArray(index);
    flushGLErrors();
}

void GLWrapper::GLDRAWARRAYS(GLenum mode, GLint first, GLsizei size)
{
    MESSAGE(5, "GL call: glDrawArrays(%d, %d, %d)\n", mode, first, size);
    glDrawArrays(mode, first, size);
    flushGLErrors();
}

void GLWrapper::GLGENTEXTURES(GLsizei size, GLuint *ptr)
{
    MESSAGE(4, "GL call: glGenTextures(%d, %p)\n", size, ptr);
    glGenTextures(size, ptr);
    flushGLErrors();
}

void GLWrapper::GLBINDTEXTURE(GLenum target, GLuint id)
{
    MESSAGE(5, "GL call: glBindTexture(%d, %d)\n", target, id);
    glBindTexture(target, id);
    flushGLErrors();
}

void GLWrapper::GLPIXELSTOREI(GLenum type, GLint align)
{
    MESSAGE(4, "GL call: glPixelStorei(%d, %d)\n", type, align);
    glPixelStorei(type, align);
    flushGLErrors();
}

void GLWrapper::GLTEXIMAGE2D(GLenum target, GLint level, GLint internalformat, GLsizei width,
                                   GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    MESSAGE(4, "GL call: glTexImage2D(%d, %d, %d, %d, %d, %d, %d, %d, %p)\n",
               target, level, internalformat, width, height, border, format, type, pixels);
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    flushGLErrors();
}

void GLWrapper::GLTEXPARAMETERI(GLenum target, GLenum pname, GLint param)
{
    MESSAGE(4, "GL call: glTexParameteri(%d, %d, %d)\n", target, pname, param);
    glTexParameteri(target, pname, param);
    flushGLErrors();
}

GLint GLWrapper::GLGETATTRIBLOCATION(GLuint program, const GLchar *name)
{
    GLint rc;
    rc = glGetAttribLocation(program, name);
    MESSAGE(4, "GL call: glGetAttribLocation(%d, '%s') = %d\n", program, name, rc);
    flushGLErrors();
    return rc;
}

GLint GLWrapper::GLGETUNIFORMLOCATION(GLuint program, const GLchar *name)
{
    GLint rc;
    rc = glGetUniformLocation(program, name);
    MESSAGE(4, "GL call: glGetUniformLocation(%d, '%s') = %d\n", program, name, rc);
    flushGLErrors();
    return rc;
}

void GLWrapper::GLACTIVETEXTURE(GLenum texture)
{
    MESSAGE(5, "GL call: glActiveTexture(%d)\n", texture);
    glActiveTexture(texture);
    flushGLErrors();
}

void GLWrapper::GLUNIFORM1I(GLint location, GLint x)
{
    MESSAGE(5, "GL call: glUniform1i(%d, %d)\n", location, x);
    glUniform1i(location, x);
    flushGLErrors();
}

void GLWrapper::GLUNIFORM1F(GLint location, GLfloat x)
{
    MESSAGE(5, "GL call: glUniform4f(%d, %f)\n", location, x);
    glUniform1f(location, x);
    flushGLErrors();
}

void GLWrapper::GLUNIFORM4F(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    MESSAGE(5, "GL call: glUniform4f(%d, %f, %f, %f, %f)\n", location, x, y, z, w);
    glUniform4f(location, x, y, z, w);
    flushGLErrors();
}

void GLWrapper::GLCOMPRESSEDTEXIMAGE2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint height, GLint border, GLsizei size, const GLvoid *data)
{
    MESSAGE(4, "GL call: glCompressedTexImage2D(0x%x, %d, 0x%x, %d, %d, %d, %d, %p)\n",
               target, level, internalformat, width, height, border, size, data);
    glCompressedTexImage2D(target, level, internalformat, width, height, border, size, data);
    flushGLErrors();
}

/******************************************************************************
 * EGL Wrappers
 */

void GLWrapper::EGLSWAPBUFFERS(EGLDisplay egl_display, EGLSurface egl_surface)
{
    MESSAGE(5, "Attempting a call to eglSwapBuffers(%p, %p)\n", egl_display, egl_surface);
    eglSwapBuffers(egl_display, egl_surface);
    flushEGLErrors();
}
