
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef b09_Terrain_H
#define b09_Terrain_H

/*************************************************************************************************
 */

#include "EGLX11Benchmark.h"

#include "SimpleShader.h"
#include "SimpleTexture.h"
#include "SimpleMesh.h"
#include "DebugLog.h"

/*************************************************************************************************
 */

typedef struct _QuadTreeNode {
    float center_x, center_y, center_z;
    float size_x, size_y, size_z;
    unsigned int nextnodes[4];
} QUADTREENODE;

/*************************************************************************************************
 */

class b09_Terrain : public EGLX11Benchmark
{
public:
    b09_Terrain();
    ~b09_Terrain();

    bool initBenchmark(void);
    bool destroyBenchmark(void);
    bool renderSingleFrame(float deltatime);
    bool getRenderStatistics(RENDER_STATISTICS *rs);

protected:
    void buildTerrainPatch(void);

private:
    SimpleShader *ss;       // For shading the terrain
    SimpleTexture *st;      // For the geometry texture (heightmap)
    SimpleMesh *sm;         // For containing a single LoDded patch of terrain

    unsigned char *hMap;
    unsigned int hMapLen;
};

#endif // b09_Terrain_H

