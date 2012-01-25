
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef GLMath_H
#define GLMath_H

#include  <GLES2/gl2.h>
#include  <GLES2/gl2ext.h>
#include  <EGL/egl.h>


typedef struct
{
    GLfloat   m[4][4];
} Matrix4X4;


class GLMath {
public:
    static GLMath * Instance();
    ~GLMath();

    void _glScale(Matrix4X4 *result, GLfloat sx, GLfloat sy, GLfloat sz);
    void _glTranslate(Matrix4X4 *result, GLfloat tx, GLfloat ty, GLfloat tz);
    void _glRotate(Matrix4X4 *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    void _glFrustum(Matrix4X4 *result, float left, float right, float bottom, float top, float nearZ, float farZ);
    void _glPerspective(Matrix4X4 *result, float fovy, float aspect, float nearZ, float farZ);
    void _glOrtho(Matrix4X4 *result, float left, float right, float bottom, float top, float nearZ, float farZ);
    void _glLoadIdentity(Matrix4X4 *result);
    void _glMatrixMultiply(Matrix4X4 *result, Matrix4X4 *srcA, Matrix4X4 *srcB);
    void _printMatrix(Matrix4X4 *result);

private:
    GLMath();                                     // Private constructor
    GLMath(GLMath const &) {};
    static GLMath *p_Instance;                    // Single instance placeholder
};

#endif // GLMath_H
