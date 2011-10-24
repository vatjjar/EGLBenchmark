
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b04_ETCTextureTest_H
#define b04_ETCTextureTest_H

#include "EGLX11Benchmark.h"

#include "GLES2/gl2ext.h"

class b04_ETCTextureTest : public EGLX11Benchmark
{
public:
    b04_ETCTextureTest();
    ~b04_ETCTextureTest();

    bool initBenchmark(unsigned int width, unsigned int height, bool fullscreen);
    bool destroyBenchmark(void);
    bool runBenchmark(float duration);
    bool displayResult(void);

private:
    bool etc1_supported;
};

#endif // b04_ETCTextureTest_H

