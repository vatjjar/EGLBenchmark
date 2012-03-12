
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b04_ETCTextureTest_H
#define b04_ETCTextureTest_H

#include "EGLX11Benchmark.h"

#include "SimpleTexture.h"
#include "SimpleShader.h"

class b04_ETCTextureTest : public EGLX11Benchmark
{
public:
    b04_ETCTextureTest();
    ~b04_ETCTextureTest();

    bool initBenchmark(void);
    bool destroyBenchmark(void);
    bool renderSingleFrame(float deltatime);
    bool getRenderStatistics(RENDER_STATISTICS *rs);

private:
    // Test case specific methods
    void Render(void);
    bool queryCompressedTextureformats(void);

    // Test case specific variables
    unsigned int renderedFrames;
    float totaltime;

    GLuint texturesampler;

    SimpleTexture *st;
    SimpleShader *ss;
};

#endif // b04_ETCTextureTest_H

