
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */


#include "EGLX11Benchmark.h"
#include "EGLX11Benchmark_GLWrappers.h"

/*
 * GL Wrappers:
 * ------------
 * The intention of these methods are to easily wrap GL calls and bundle them with something
 * useful, like debug information and error grabbing. This way seems most suitable for the
 * purpose.
 */

void EGLX11Benchmark::GLATTACHSHADER(GLuint shaderProgram, GLuint shader)
{
    MESSAGE(4, "GL call: glAttachShader(%d, %d)\n", shaderProgram, shader);
    glAttachShader(shaderProgram, shader);
    flushGLErrors();
}

void EGLX11Benchmark::GLBINDATTRIBLOCATION(GLuint shaderProgram, GLuint index, const GLchar *name)
{
    MESSAGE(4, "GL call: glBindAttribLocation(%d, %d, '%s')\n", shaderProgram, index, name);
    glBindAttribLocation(shaderProgram, index, name);
    flushGLErrors();
}
void EGLX11Benchmark::GLCLEARCOLOR(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
    MESSAGE(4, "GL call: glClearColor(%f, %f, %f, %f)\n", r, g, b, a);
    glClearColor(r, g, b, a);
    flushGLErrors();
}

GLuint EGLX11Benchmark::GLCREATEPROGRAM(void)
{
    GLuint program;
    program = glCreateProgram();
    MESSAGE(4, "GL call: glCreateProgram() = %d\n", program);
    flushGLErrors();
    return program;
}

void EGLX11Benchmark::GLLINKPROGRAM(GLuint program)
{
    MESSAGE(4, "GL call: glLinkProgram(%d)\n", program);
    glLinkProgram(program);
    flushGLErrors();
}

void EGLX11Benchmark::GLUSEPROGRAM(GLuint program)
{
    MESSAGE(5, "GL call: glUseProgram(%d)\n", program);
    glUseProgram(program);
    flushGLErrors();
}

void EGLX11Benchmark::GLVIEWPORT(GLint x, GLint y, GLsizei width, GLsizei height)
{
    MESSAGE(5, "GL call: glViewPort(%d, %d, %d, %d)\n", x, y, width, height);
    glViewport(x, y, width, height);
    flushGLErrors();
}

void EGLX11Benchmark::GLCLEAR(GLbitfield mask)
{
    MESSAGE(5, "GL call: glClear(0x%x)\n", mask);
    glClear(mask);
    flushGLErrors();
}

void EGLX11Benchmark::GLVERTEXATTRIBPOINTER(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *data)
{
    MESSAGE(5, "GL call: glVertexAttribPointer(%d, %d, %d, %d, %d, %p)\n",
               index, size, type, normalized, stride, data);
    glVertexAttribPointer(index, size, type, normalized, stride, data);
    flushGLErrors();
}

void EGLX11Benchmark::GLENABLEVERTEXATTRIBARRAY(GLuint index)
{
    MESSAGE(5, "GL call: glEnableVertexAttribPointer(%d)\n", index);
    glEnableVertexAttribArray(index);
    flushGLErrors();
}

void EGLX11Benchmark::GLDRAWARRAYS(GLenum mode, GLint first, GLsizei size)
{
    MESSAGE(5, "GL call: glDrawArrays(%d, %d, %d)\n", mode, first, size);
    glDrawArrays(mode, first, size);
    flushGLErrors();
}

void EGLX11Benchmark::GLGENTEXTURES(GLsizei size, GLuint *ptr)
{
    MESSAGE(4, "GL call: glGenTextures(%d, %p)\n", size, ptr);
    glGenTextures(size, ptr);
    flushGLErrors();
}

void EGLX11Benchmark::GLBINDTEXTURE(GLenum target, GLuint id)
{
    MESSAGE(5, "GL call: glBindTexture(%d, %d)\n", target, id);
    glBindTexture(target, id);
    flushGLErrors();
}

void EGLX11Benchmark::GLPIXELSTOREI(GLenum type, GLint align)
{
    MESSAGE(4, "GL call: glPixelStorei(%d, %d)\n", type, align);
    glPixelStorei(type, align);
    flushGLErrors();
}

void EGLX11Benchmark::GLTEXIMAGE2D(GLenum target, GLint level, GLint internalformat, GLsizei width,
                                   GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    MESSAGE(4, "GL call: glTexImage2D(%d, %d, %d, %d, %d, %d, %d, %d, %p)\n",
               target, level, internalformat, width, height, border, format, type, pixels);
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    flushGLErrors();
}

void EGLX11Benchmark::GLTEXPARAMETERI(GLenum target, GLenum pname, GLint param)
{
    MESSAGE(4, "GL call: glTexParameteri(%d, %d, %d)\n", target, pname, param);
    glTexParameteri(target, pname, param);
    flushGLErrors();
}

GLint EGLX11Benchmark::GLGETATTRIBLOCATION(GLuint program, const GLchar *name)
{
    GLint rc;
    rc = glGetAttribLocation(program, name);
    MESSAGE(4, "GL call: glGetAttribLocation(%d, '%s') = %d\n", program, name, rc);
    flushGLErrors();
    return rc;
}

GLint EGLX11Benchmark::GLGETUNIFORMLOCATION(GLuint program, const GLchar *name)
{
    GLint rc;
    rc = glGetUniformLocation(program, name);
    MESSAGE(4, "GL call: glGetUniformLocation(%d, '%s') = %d\n", program, name, rc);
    flushGLErrors();
    return rc;
}

void EGLX11Benchmark::GLACTIVETEXTURE(GLenum texture)
{
    MESSAGE(5, "GL call: glActiveTexture(%d)\n", texture);
    glActiveTexture(texture);
    flushGLErrors();
}

void EGLX11Benchmark::GLUNIFORM1I(GLint location, GLint x)
{
    MESSAGE(5, "GL call: glUniform1i(%d, %d)\n", location, x);
    glUniform1i(location, x);
    flushGLErrors();
}

void EGLX11Benchmark::GLUNIFORM1F(GLint location, GLfloat x)
{
    MESSAGE(5, "GL call: glUniform4f(%d, %f)\n", location, x);
    glUniform1f(location, x);
    flushGLErrors();
}

void EGLX11Benchmark::GLUNIFORM4F(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    MESSAGE(5, "GL call: glUniform4f(%d, %f, %f, %f, %f)\n", location, x, y, z, w);
    glUniform4f(location, x, y, z, w);
    flushGLErrors();
}

void EGLX11Benchmark::GLCOMPRESSEDTEXIMAGE2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint height, GLint border, GLsizei size, const GLvoid *data)
{
    MESSAGE(4, "GL call: glCompressedTexImage2D(0x%x, %d, 0x%x, %d, %d, %d, %d, %p)\n",
               target, level, internalformat, width, height, border, size, data);
    glCompressedTexImage2D(target, level, internalformat, width, height, border, size, data);
    flushGLErrors();
}

