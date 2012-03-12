
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef EGLX11Benchmark_H
#define EGLX11Benchmark_H

#include <iostream>

#include  <string.h>
#include  <stdio.h>

#include  <GLES2/gl2.h>
#include  <GLES2/gl2ext.h>
#include  <EGL/egl.h>

#include "EGLX11Display.h"
#include "GLWrapper.h"

typedef struct _render_statistics {
    unsigned int r_vertices;
    unsigned int r_normals;
    unsigned int r_texcoords;
    unsigned int r_faces;
    unsigned int r_batches;
} RENDER_STATISTICS;

enum {
    TEST_CONTEXTINIT = 1,
    TEST_SIMPLESHADER,
    TEST_SIMPLETRIANGLE,
    TEST_ETCTEXTURE,
    TEST_RGBTEXTURE,
    TEST_VBOELEMENTSRGB,
    TEST_POINTCLOUD,
    TEST_SCENEGRAPH,
    TEST_TERRAIN
};

// Class definition

class EGLX11Benchmark
{
public:
    EGLX11Benchmark();
    ~EGLX11Benchmark();

public:
    virtual bool initBenchmark(void) = 0;
    virtual bool destroyBenchmark(void) = 0;
    virtual bool renderSingleFrame(float deltatime) = 0;
    virtual bool getRenderStatistics(RENDER_STATISTICS *rs) = 0;
    virtual bool keyHandler(char text) {
        if (text == 033) return true;  /* ESC */
        return false;
    }

    // Getters for benchmark naming info
    const char * getName(void);
    const char * getDescription(void);

    // user interruption:
    bool userInterrupt(void);

    // Display handle passing
    int setEGLDisplay(EGLX11Display *d);

protected:
    void setName(const char *);
    void setDescription(const char *);

    // Benchmark case name and description
    const char * name;
    const char * description;

    EGLX11Display *display;

private:
};

#endif // EGLX11Benchmark_H

