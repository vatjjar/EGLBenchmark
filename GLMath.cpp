
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "GLMath.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

/******************************************************************************
 * Global static instance of the class
 */

GLMath * GLMath::p_Instance = NULL;

GLMath * GLMath::Instance()
{
    if (p_Instance == NULL)
    {
        p_Instance = new GLMath();
    }
    return p_Instance;
}

GLMath::GLMath()
{
}

GLMath::~ GLMath()
{
}

/******************************************************************************
 * Public GLMath methods
 */

void GLMath::_glScale(Matrix4X4 *result, GLfloat sx, GLfloat sy, GLfloat sz)
{
    result->m[0][0] *= sx;
    result->m[0][1] *= sx;
    result->m[0][2] *= sx;
    result->m[0][3] *= sx;

    result->m[1][0] *= sy;
    result->m[1][1] *= sy;
    result->m[1][2] *= sy;
    result->m[1][3] *= sy;

    result->m[2][0] *= sz;
    result->m[2][1] *= sz;
    result->m[2][2] *= sz;
    result->m[2][3] *= sz;
}

void GLMath::_glTranslate(Matrix4X4 *result, GLfloat tx, GLfloat ty, GLfloat tz)
{
    result->m[3][0] += (result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
    result->m[3][1] += (result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
    result->m[3][2] += (result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
    result->m[3][3] += (result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

void GLMath::_glRotate(Matrix4X4 *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
   GLfloat sinAngle, cosAngle;
   GLfloat mag = sqrtf(x * x + y * y + z * z);

   sinAngle = sinf ( angle * M_PI / 180.0f );
   cosAngle = cosf ( angle * M_PI / 180.0f );
   if ( mag > 0.0f )
   {
      GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
      GLfloat oneMinusCos;
      Matrix4X4 rotMat;

      x /= mag;
      y /= mag;
      z /= mag;

      xx = x * x;
      yy = y * y;
      zz = z * z;
      xy = x * y;
      yz = y * z;
      zx = z * x;
      xs = x * sinAngle;
      ys = y * sinAngle;
      zs = z * sinAngle;
      oneMinusCos = 1.0f - cosAngle;

      rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
      rotMat.m[0][1] = (oneMinusCos * xy) - zs;
      rotMat.m[0][2] = (oneMinusCos * zx) + ys;
      rotMat.m[0][3] = 0.0F;

      rotMat.m[1][0] = (oneMinusCos * xy) + zs;
      rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
      rotMat.m[1][2] = (oneMinusCos * yz) - xs;
      rotMat.m[1][3] = 0.0F;

      rotMat.m[2][0] = (oneMinusCos * zx) - ys;
      rotMat.m[2][1] = (oneMinusCos * yz) + xs;
      rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
      rotMat.m[2][3] = 0.0F;

      rotMat.m[3][0] = 0.0F;
      rotMat.m[3][1] = 0.0F;
      rotMat.m[3][2] = 0.0F;
      rotMat.m[3][3] = 1.0F;

      _glMatrixMultiply( result, &rotMat, result );
   }
}

void GLMath::_glFrustum(Matrix4X4 *result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = farZ - nearZ;
    Matrix4X4    frust;

    if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
         (deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
         return;

    frust.m[0][0] = 2.0f * nearZ / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

    frust.m[1][1] = 2.0f * nearZ / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

    frust.m[2][0] = (right + left) / deltaX;
    frust.m[2][1] = (top + bottom) / deltaY;
    frust.m[2][2] = -(nearZ + farZ) / deltaZ;
    frust.m[2][3] = -1.0f;

    frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

    _glMatrixMultiply(result, &frust, result);
}


void GLMath::_glPerspective(Matrix4X4 *result, float fovy, float aspect, float nearZ, float farZ)
{
   GLfloat frustumW, frustumH;

   frustumH = tanf( fovy / 360.0f * M_PI ) * nearZ;
   frustumW = frustumH * aspect;

   _glFrustum( result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
}

void GLMath::_glOrtho(Matrix4X4 *result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = farZ - nearZ;
    Matrix4X4    ortho;

    if ( (deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f) )
        return;

    _glLoadIdentity(&ortho);
    ortho.m[0][0] = 2.0f / deltaX;
    ortho.m[3][0] = -(right + left) / deltaX;
    ortho.m[1][1] = 2.0f / deltaY;
    ortho.m[3][1] = -(top + bottom) / deltaY;
    ortho.m[2][2] = -2.0f / deltaZ;
    ortho.m[3][2] = -(nearZ + farZ) / deltaZ;

    _glMatrixMultiply(result, &ortho, result);
}


void GLMath::_glMatrixMultiply(Matrix4X4 *result, Matrix4X4 *srcA, Matrix4X4 *srcB)
{
    Matrix4X4    tmp;
    int         i;

        for (i=0; i<4; i++)
        {
                tmp.m[i][0] =	(srcA->m[i][0] * srcB->m[0][0]) +
                                                (srcA->m[i][1] * srcB->m[1][0]) +
                                                (srcA->m[i][2] * srcB->m[2][0]) +
                                                (srcA->m[i][3] * srcB->m[3][0]) ;

                tmp.m[i][1] =	(srcA->m[i][0] * srcB->m[0][1]) +
                                                (srcA->m[i][1] * srcB->m[1][1]) +
                                                (srcA->m[i][2] * srcB->m[2][1]) +
                                                (srcA->m[i][3] * srcB->m[3][1]) ;

                tmp.m[i][2] =	(srcA->m[i][0] * srcB->m[0][2]) +
                                                (srcA->m[i][1] * srcB->m[1][2]) +
                                                (srcA->m[i][2] * srcB->m[2][2]) +
                                                (srcA->m[i][3] * srcB->m[3][2]) ;

                tmp.m[i][3] =	(srcA->m[i][0] * srcB->m[0][3]) +
                                                (srcA->m[i][1] * srcB->m[1][3]) +
                                                (srcA->m[i][2] * srcB->m[2][3]) +
                                                (srcA->m[i][3] * srcB->m[3][3]) ;
        }
    memcpy(result, &tmp, sizeof(Matrix4X4));
}


void GLMath::_glLoadIdentity(Matrix4X4 *result)
{
    memset(result, 0x0, sizeof(Matrix4X4));
    result->m[0][0] = 1.0f;
    result->m[1][1] = 1.0f;
    result->m[2][2] = 1.0f;
    result->m[3][3] = 1.0f;
}

void GLMath::_printMatrix(Matrix4X4 *result)
{
    for (int i=0; i<4; i++) std::cout << result->m[0][i] << " "; std::cout << "\n";
    for (int i=0; i<4; i++) std::cout << result->m[1][i] << " "; std::cout << "\n";
    for (int i=0; i<4; i++) std::cout << result->m[2][i] << " "; std::cout << "\n";
    for (int i=0; i<4; i++) std::cout << result->m[3][i] << " "; std::cout << "\n";
}
