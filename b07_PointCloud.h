
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b07_PointCloud_H
#define b07_PointCloud_H

#include "EGLX11Benchmark.h"

#include "SimpleShader.h"
#include "DebugLog.h"

class b07_PointCloud : public EGLX11Benchmark
{
public:
    b07_PointCloud();
    ~b07_PointCloud();

    bool initBenchmark(unsigned int width, unsigned int height, bool fullscreen);
    bool destroyBenchmark(void);
    bool renderSingleFrame(float deltatime);
    bool getRenderStatistics(RENDER_STATISTICS *rs);

private:
    SimpleShader *ss;
};

#endif // b07_PointCloud_H

