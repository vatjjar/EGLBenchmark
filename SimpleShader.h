
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef SimpleShader_H
#define SimpleShader_H

#include  <GLES2/gl2.h>
#include  <GLES2/gl2ext.h>
#include  <EGL/egl.h>

#include <string>

#include "GLWrapper.h"

// Class definition

class SimpleShader
{
public:
    SimpleShader();
    ~SimpleShader();

public:
    /* File I/O */
    bool fromFiles(const char *v_src, const char *f_src);
    void bindProgram(void);
    void linkProgram(void);
    void destroyProgram(void);

    GLuint getProgramObject(void);

protected:

private:
    // Helper methods for shader handling
    GLuint createShaderProgram(const char *v_src, const char *f_src);
    GLuint loadShaderProgram(const char *shader_source, GLenum type);
    void linkShaderProgram(GLuint shaderProgram);
    void printShaderInfo(GLuint shader);

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
};

#endif // SimpleShader_H
