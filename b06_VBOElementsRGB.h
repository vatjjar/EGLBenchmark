
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b06_VBOElementsRGB_H
#define b06_VBOElementsRGB_H

#include "EGLX11Benchmark.h"

#include "SimpleMesh.h"
#include "SimpleTexture.h"

#define TESTOBJECTS (5)

class b06_VBOElementsRGB : public EGLX11Benchmark
{
public:
    b06_VBOElementsRGB();
    ~b06_VBOElementsRGB();

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

    GLuint shaderProgram;
    GLuint texturesampler;

    // Mesh and texture istances for the test case
    SimpleMesh    *sm[TESTOBJECTS];
    SimpleTexture *st[TESTOBJECTS];
};

#endif // b06_VBOElementsRGB_H

