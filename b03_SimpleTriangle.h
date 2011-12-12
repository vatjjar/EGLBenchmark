
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b03_SimpleTriangle_H
#define b03_SimpleTriangle_H

#include "EGLX11Benchmark.h"

class b03_SimpleTriangle : public EGLX11Benchmark
{
public:
    b03_SimpleTriangle();
    ~b03_SimpleTriangle();

public:
    bool initBenchmark(unsigned int width, unsigned int height, bool fullscreen);
    bool destroyBenchmark(void);
    bool renderSingleFrame(float deltatime);
    bool getRenderStatistics(RENDER_STATISTICS *rs);

private:
    // Test case specific methods
    void Render(void);

    // Test case specific variables
    unsigned int renderedFrames;
    float totaltime;

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
};

#endif // b03_SimpleTriangle_H
