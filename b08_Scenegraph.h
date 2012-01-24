
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b08_Scenegraph_H
#define b08_Scenegraph_H

#include "EGLX11Benchmark.h"

#include "SimpleShader.h"
#include "SimpleScenegraph.h"
#include "DebugLog.h"

typedef struct
{
    GLfloat   m[4][4];
} Matrix4x4;


class b08_Scenegraph : public EGLX11Benchmark
{
public:
    b08_Scenegraph();
    ~b08_Scenegraph();

    bool initBenchmark(unsigned int width, unsigned int height, bool fullscreen);
    bool destroyBenchmark(void);
    bool renderSingleFrame(float deltatime);
    bool getRenderStatistics(RENDER_STATISTICS *rs);
    bool keyHandler(char test);

private:
    void _glScale(Matrix4x4 *result, GLfloat sx, GLfloat sy, GLfloat sz);
    void _glTranslate(Matrix4x4 *result, GLfloat tx, GLfloat ty, GLfloat tz);
    void _glRotate(Matrix4x4 *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    void _glFrustum(Matrix4x4 *result, float left, float right, float bottom, float top, float nearZ, float farZ);
    void _glPerspective(Matrix4x4 *result, float fovy, float aspect, float nearZ, float farZ);
    void _glOrtho(Matrix4x4 *result, float left, float right, float bottom, float top, float nearZ, float farZ);
    void _glLoadIdentity(Matrix4x4 *result);

    void _glMatrixMultiply(Matrix4x4 *result, Matrix4x4 *srcA, Matrix4x4 *srcB);

    void _printMatrix(Matrix4x4 *result);

    SimpleScenegraph *ssg;
    SimpleShader *ss;

    GLuint u_matrix;
    GLfloat camera_x, camera_y, camera_z;
};

#endif // b08_Scenegraph_H

