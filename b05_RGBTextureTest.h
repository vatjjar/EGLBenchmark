
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b05_RGBTextureTest_H
#define b05_RGBTextureTest_H

#include "EGLX11Benchmark.h"

#include "SimpleTexture.h"
#include "SimpleShader.h"

class b05_RGBTextureTest : public EGLX11Benchmark
{
public:
    b05_RGBTextureTest();
    ~b05_RGBTextureTest();

    bool initBenchmark();
    bool destroyBenchmark(void);
    bool renderSingleFrame(float deltatime);
    bool getRenderStatistics(RENDER_STATISTICS *rs);

private:
    // Test case specific methods
    void Render(void);

    // Test case specific variables
    unsigned int renderedFrames;
    float totaltime;

    GLuint texturesampler;

    SimpleTexture *st;
    SimpleShader *ss;
};

#endif // b05_RGBTextureTest_H

