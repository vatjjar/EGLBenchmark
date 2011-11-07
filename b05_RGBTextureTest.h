
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b05_RGBTextureTest_H
#define b05_RGBTextureTest_H

#include "EGLX11Benchmark.h"

class b05_RGBTextureTest : public EGLX11Benchmark
{
public:
    b05_RGBTextureTest();
    ~b05_RGBTextureTest();

    bool initBenchmark(unsigned int width, unsigned int height, bool fullscreen);
    bool destroyBenchmark(void);
    bool displayResult(void);
    bool renderSingleFrame(float deltatime);

private:
    // Test case specific methods
    void Render(void);

    // Test case specific variables
    unsigned int renderedFrames;
    float totaltime;

    GLuint shaderProgram;
    GLuint textureID;
    GLuint texturesampler;
};

#endif // b05_RGBTextureTest_H

