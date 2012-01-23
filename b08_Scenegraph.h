
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b08_Scenegraph_H
#define b08_Scenegraph_H

#include "EGLX11Benchmark.h"

#include "SimpleShader.h"
#include "DebugLog.h"

class b08_Scenegraph : public EGLX11Benchmark
{
public:
    b08_Scenegraph();
    ~b08_Scenegraph();

    bool initBenchmark(unsigned int width, unsigned int height, bool fullscreen);
    bool destroyBenchmark(void);
    bool renderSingleFrame(float deltatime);
    bool getRenderStatistics(RENDER_STATISTICS *rs);

private:
    SimpleShader *ss;
};

#endif // b08_Scenegraph_H

