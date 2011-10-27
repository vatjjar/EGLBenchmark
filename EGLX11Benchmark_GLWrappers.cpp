
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
    MESSAGE_2P(4, "Attempting a call to glAttachShader(%d, %d)\n", shaderProgram, shader);
    glAttachShader(shaderProgram, shader);
    flushGLErrors();
}

void EGLX11Benchmark::GLBINDATTRIBLOCATION(GLuint shaderProgram, GLuint index, const GLchar *name)
{
    MESSAGE_3P(4, "Attempting a call to glBindAttribLocation(%d, %d, '%s')\n", shaderProgram, index, name);
    glBindAttribLocation(shaderProgram, index, name);
    flushGLErrors();
}

void EGLX11Benchmark::GLCLEARCOLOR(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
    MESSAGE_4P(4, "Attempting a call to glClearColor(%f, %f, %f, %f)\n", r, g, b, a);
    glClearColor(r, g, b, a);
    flushGLErrors();
}

GLuint EGLX11Benchmark::GLCREATEPROGRAM(void)
{
    GLuint program;
    MESSAGE(4, "Attempting a call to glCreateProgram()\n");
    program = glCreateProgram();
    flushGLErrors();
    return program;
}

void EGLX11Benchmark::GLLINKPROGRAM(GLuint program)
{
    MESSAGE_1P(4, "Attempting a call to glLinkProgram(%d)\n", program);
    glLinkProgram(program);
    flushGLErrors();
}

void EGLX11Benchmark::GLUSEPROGRAM(GLuint program)
{
    MESSAGE_1P(5, "Attempting a call to glUseProgram(%d)\n", program);
    glUseProgram(program);
    flushGLErrors();
}

void EGLX11Benchmark::GLVIEWPORT(GLint x, GLint y, GLsizei width, GLsizei height)
{
    MESSAGE_4P(5, "Attempting a call to glViewPort(%d, %d, %d, %d)\n", x, y, width, height);
    glViewport(x, y, width, height);
    flushGLErrors();
}

void EGLX11Benchmark::GLCLEAR(GLbitfield mask)
{
    MESSAGE_1P(5, "Attempting a call to glClear(0x%x)\n", mask);
    glClear(mask);
    flushGLErrors();
}

void EGLX11Benchmark::GLVERTEXATTRIBPOINTER(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *data)
{
    MESSAGE_6P(5, "Attempting a call to glVertexAttribPointer(%d, %d, %d, %d, %d, %p)\n",
               index, size, type, normalized, stride, data);
    glVertexAttribPointer(index, size, type, normalized, stride, data);
    flushGLErrors();
}

void EGLX11Benchmark::GLENABLEVERTEXATTRIBARRAY(GLuint index)
{
    MESSAGE_1P(5, "Attempting a call to glEnableVertexAttribPointer(%d)\n", index);
    glEnableVertexAttribArray(index);
    flushGLErrors();
}

void EGLX11Benchmark::GLDRAWARRAYS(GLenum mode, GLint first, GLsizei size)
{
    MESSAGE_3P(5, "Attempting a call to glDrawArrays(%d, %d, %d)\n", mode, first, size);
    glDrawArrays(mode, first, size);
    flushGLErrors();
}

void EGLX11Benchmark::GLGENTEXTURES(GLsizei size, GLuint *ptr)
{
    MESSAGE_2P(4, "Attempting a call to glGenTextures(%d, %p)\n", size, ptr);
    glGenTextures(size, ptr);
    flushGLErrors();
}

void EGLX11Benchmark::GLBINDTEXTURE(GLenum target, GLuint id)
{
    MESSAGE_2P(4, "Attempting a call to glBindTexture(%d, %d)\n", target, id);
    glBindTexture(target, id);
    flushGLErrors();
}

void EGLX11Benchmark::GLPIXELSTOREI(GLenum type, GLint align)
{
    MESSAGE_2P(4, "Attempting a call to glPixelStorei(%d, %d)\n", type, align);
    glPixelStorei(type, align);
    flushGLErrors();
}

void EGLX11Benchmark::GLTEXIMAGE2D(GLenum target, GLint level, GLint internalformat, GLsizei width,
                                   GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    MESSAGE_6P(4, "Attempting a call to glTexImage2D(%d, %d, %d, %d, %d, %d",
               target, level, internalformat, width, height, border);
    MESSAGE_3P(4, ", %d, %d, %p)\n", format, type, pixels);
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    flushGLErrors();
}

void EGLX11Benchmark::GLTEXPARAMETERI(GLenum target, GLenum pname, GLint param)
{
    MESSAGE_3P(3, "Attempting a call to glTexParameteri(%d, %d, %d)\n", target, pname, param);
    glTexParameteri(target, pname, param);
    flushGLErrors();
}
