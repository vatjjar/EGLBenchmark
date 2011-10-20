
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b02_SimpleGLShading_H
#define b02_SimpleGLShading_H

#include "EGLX11Benchmark.h"

class b02_SimpleGLShading : public EGLX11Benchmark
{
public:
    b02_SimpleGLShading();
    ~b02_SimpleGLShading();

public:
    bool initBenchmark(unsigned int width, unsigned int height, bool fullscreen);
    bool destroyBenchmark(void);
    bool runBenchmark(float duration);
    bool displayResult(void);

private:
    // Test case specific methods
    void Render(void);

    // Test case specific variables
    unsigned int renderedFrames;
    float totaltime;

    // Variables for the animation in this test case
    float phase;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLint phase_loc;
    GLint offset_loc;
    GLint position_loc;
};

#endif // b02_SimpleGLShading_H
