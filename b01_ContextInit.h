
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef B01_CONTEXTINIT_H
#define B01_CONTEXTINIT_H

#include "EGLX11Benchmark.h"

class b01_ContextInit : public EGLX11Benchmark
{
public:
    b01_ContextInit();
    ~b01_ContextInit();

    bool initBenchmark(unsigned int width, unsigned int height, bool fullscreen);
    bool destroyBenchmark(void);
    bool renderSingleFrame(float deltatime);
    bool getRenderStatistics(RENDER_STATISTICS *rs);
};

#endif // B01_CONTEXTINIT_H

