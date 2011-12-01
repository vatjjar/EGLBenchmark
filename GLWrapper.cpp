
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */


#include "GLWrapper.h"
#include "DebugLog.h"

#include <iostream>

/*
 * GL Wrappers:
 * ------------
 * The intention of these methods are to easily wrap GL calls and bundle them with something
 * useful, like debug information and error grabbing. This way seems most suitable for the
 * purpose.
 */

GLWrapper::GLWrapper() :
    GLerrors(0),
    EGLerrors(0)
{
}

GLWrapper::~GLWrapper()
{
}

/*
 *
 */

void GLWrapper::flushGLErrors(void)
{
    GLuint error;
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        GLerrors++;
        std::cout << "GL ERROR: glGetError() returned " << error;
        switch(error)
        {
        case GL_INVALID_VALUE:
            std::cout << " (GL_INVALID_VALUE)\n";
            break;
        case GL_INVALID_ENUM:
            std::cout << " (GL_INVALID_ENUM)\n";
            break;
        case GL_INVALID_OPERATION:
            std::cout << " (GL_INVALID_OPERATION)\n";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << " (GL_INVALID_FRAMEBUFFER_OPERATION) ";
            std::cout << "glCheckFramebufferStatus(GL_FRAMEBUFFER) = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n";
            break;
        default:
            std::cout << " (UNKNOWN GL ERROR)\n";
            break;
        }
    }
}

void GLWrapper::flushEGLErrors(void)
{
    GLuint error;
    error = eglGetError();
    if (error != EGL_SUCCESS)
    {
        EGLerrors++;
        std::cout << "EGL ERROR: eglGetError() returned " << error;
        switch(error)
        {
        case EGL_NOT_INITIALIZED:
            std::cout << " (EGL_NOT_INITIALIZED)\n";
            break;
        case EGL_BAD_ACCESS:
            std::cout << " (EGL_BAD_ACCESS)\n";
            break;
        case EGL_BAD_ALLOC:
            std::cout << " (EGL_BAD_ALLOC)\n";
            break;
        case EGL_BAD_ATTRIBUTE:
            std::cout << " (EGL_BAD_ATTRIBUTE) ";
            break;
        case EGL_BAD_CONTEXT:
            std::cout << " (EGL_BAD_CONTEXT) ";
            break;
        case EGL_BAD_CONFIG:
            std::cout << " (EGL_BAD_CONFIG) ";
            break;
        case EGL_BAD_CURRENT_SURFACE:
            std::cout << " (EGL_BAD_CURRENT_SURFACE) ";
            break;
        case EGL_BAD_DISPLAY:
            std::cout << " (EGL_BAD_DISPLAY) ";
            break;
        case EGL_BAD_SURFACE:
            std::cout << " (EGL_BAD_SURFACE) ";
            break;
        case EGL_BAD_MATCH:
            std::cout << " (EGL_BAD_MATCH) ";
            break;
        case EGL_BAD_PARAMETER:
            std::cout << " (EGL_BAD_PARAMETER) ";
            break;
        case EGL_BAD_NATIVE_PIXMAP:
            std::cout << " (EGL_BAD_NATIVE_PIXMAP) ";
            break;
        case EGL_BAD_NATIVE_WINDOW:
            std::cout << " (EGL_BAD_NATIVE_WINDOW) ";
            break;
        default:
            std::cout << " (UNKNOWN EGL ERROR)\n";
            break;
        }
    }
}

unsigned int GLWrapper::getGLErrors(void)
{
    return GLerrors;
}

unsigned int GLWrapper::getEGLErrors(void)
{
    return EGLerrors;
}

/*
 *
 */

void GLWrapper::GLATTACHSHADER(GLuint shaderProgram, GLuint shader)
{
    DebugLog::Instance()->MESSAGE(4, "GL call: glAttachShader(%d, %d)\n", shaderProgram, shader);
    glAttachShader(shaderProgram, shader);
    flushGLErrors();
}

void GLWrapper::GLBINDATTRIBLOCATION(GLuint shaderProgram, GLuint index, const GLchar *name)
{
    DebugLog::Instance()->MESSAGE(4, "GL call: glBindAttribLocation(%d, %d, '%s')\n", shaderProgram, index, name);
    glBindAttribLocation(shaderProgram, index, name);
    flushGLErrors();
}
void GLWrapper::GLCLEARCOLOR(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
    DebugLog::Instance()->MESSAGE(4, "GL call: glClearColor(%f, %f, %f, %f)\n", r, g, b, a);
    glClearColor(r, g, b, a);
    flushGLErrors();
}

GLuint GLWrapper::GLCREATEPROGRAM(void)
{
    GLuint program;
    program = glCreateProgram();
    DebugLog::Instance()->MESSAGE(4, "GL call: glCreateProgram() = %d\n", program);
    flushGLErrors();
    return program;
}

void GLWrapper::GLLINKPROGRAM(GLuint program)
{
    DebugLog::Instance()->MESSAGE(4, "GL call: glLinkProgram(%d)\n", program);
    glLinkProgram(program);
    flushGLErrors();
}

void GLWrapper::GLUSEPROGRAM(GLuint program)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glUseProgram(%d)\n", program);
    glUseProgram(program);
    flushGLErrors();
}

void GLWrapper::GLVIEWPORT(GLint x, GLint y, GLsizei width, GLsizei height)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glViewPort(%d, %d, %d, %d)\n", x, y, width, height);
    glViewport(x, y, width, height);
    flushGLErrors();
}

void GLWrapper::GLCLEAR(GLbitfield mask)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glClear(0x%x)\n", mask);
    glClear(mask);
    flushGLErrors();
}

void GLWrapper::GLVERTEXATTRIBPOINTER(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *data)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glVertexAttribPointer(%d, %d, %d, %d, %d, %p)\n",
               index, size, type, normalized, stride, data);
    glVertexAttribPointer(index, size, type, normalized, stride, data);
    flushGLErrors();
}

void GLWrapper::GLENABLEVERTEXATTRIBARRAY(GLuint index)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glEnableVertexAttribPointer(%d)\n", index);
    glEnableVertexAttribArray(index);
    flushGLErrors();
}

void GLWrapper::GLDRAWARRAYS(GLenum mode, GLint first, GLsizei size)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glDrawArrays(%d, %d, %d)\n", mode, first, size);
    glDrawArrays(mode, first, size);
    flushGLErrors();
}

void GLWrapper::GLGENTEXTURES(GLsizei size, GLuint *ptr)
{
    DebugLog::Instance()->MESSAGE(4, "GL call: glGenTextures(%d, %p)\n", size, ptr);
    glGenTextures(size, ptr);
    flushGLErrors();
}

void GLWrapper::GLBINDTEXTURE(GLenum target, GLuint id)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glBindTexture(%d, %d)\n", target, id);
    glBindTexture(target, id);
    flushGLErrors();
}

void GLWrapper::GLPIXELSTOREI(GLenum type, GLint align)
{
    DebugLog::Instance()->MESSAGE(4, "GL call: glPixelStorei(%d, %d)\n", type, align);
    glPixelStorei(type, align);
    flushGLErrors();
}

void GLWrapper::GLTEXIMAGE2D(GLenum target, GLint level, GLint internalformat, GLsizei width,
                                   GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    DebugLog::Instance()->MESSAGE(4, "GL call: glTexImage2D(%d, %d, %d, %d, %d, %d, %d, %d, %p)\n",
               target, level, internalformat, width, height, border, format, type, pixels);
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    flushGLErrors();
}

void GLWrapper::GLTEXPARAMETERI(GLenum target, GLenum pname, GLint param)
{
    DebugLog::Instance()->MESSAGE(4, "GL call: glTexParameteri(%d, %d, %d)\n", target, pname, param);
    glTexParameteri(target, pname, param);
    flushGLErrors();
}

GLint GLWrapper::GLGETATTRIBLOCATION(GLuint program, const GLchar *name)
{
    GLint rc;
    rc = glGetAttribLocation(program, name);
    DebugLog::Instance()->MESSAGE(4, "GL call: glGetAttribLocation(%d, '%s') = %d\n", program, name, rc);
    flushGLErrors();
    return rc;
}

GLint GLWrapper::GLGETUNIFORMLOCATION(GLuint program, const GLchar *name)
{
    GLint rc;
    rc = glGetUniformLocation(program, name);
    DebugLog::Instance()->MESSAGE(4, "GL call: glGetUniformLocation(%d, '%s') = %d\n", program, name, rc);
    flushGLErrors();
    return rc;
}

void GLWrapper::GLACTIVETEXTURE(GLenum texture)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glActiveTexture(%d)\n", texture);
    glActiveTexture(texture);
    flushGLErrors();
}

void GLWrapper::GLUNIFORM1I(GLint location, GLint x)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glUniform1i(%d, %d)\n", location, x);
    glUniform1i(location, x);
    flushGLErrors();
}

void GLWrapper::GLUNIFORM1F(GLint location, GLfloat x)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glUniform4f(%d, %f)\n", location, x);
    glUniform1f(location, x);
    flushGLErrors();
}

void GLWrapper::GLUNIFORM4F(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    DebugLog::Instance()->MESSAGE(5, "GL call: glUniform4f(%d, %f, %f, %f, %f)\n", location, x, y, z, w);
    glUniform4f(location, x, y, z, w);
    flushGLErrors();
}

void GLWrapper::GLCOMPRESSEDTEXIMAGE2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint height, GLint border, GLsizei size, const GLvoid *data)
{
    DebugLog::Instance()->MESSAGE(4, "GL call: glCompressedTexImage2D(0x%x, %d, 0x%x, %d, %d, %d, %d, %p)\n",
               target, level, internalformat, width, height, border, size, data);
    glCompressedTexImage2D(target, level, internalformat, width, height, border, size, data);
    flushGLErrors();
}

void GLWrapper::GLDRAWELEMENTS(GLenum mode, GLsizei count, GLenum type, const GLvoid *data)
{
    DebugLog::Instance()->MESSAGE(4, "GL call: glDrawElements(%d, %d, %d, %p)\n", mode, count, type, data);
    glDrawElements(mode, count, type, data);
    flushGLErrors();
}

/******************************************************************************
 * EGL Wrappers
 */

void GLWrapper::EGLSWAPBUFFERS(EGLDisplay egl_display, EGLSurface egl_surface)
{
    DebugLog::Instance()->MESSAGE(5, "Attempting a call to eglSwapBuffers(%p, %p)\n", egl_display, egl_surface);
    eglSwapBuffers(egl_display, egl_surface);
    flushEGLErrors();
}
