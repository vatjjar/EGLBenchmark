
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef SimpleScenegraph_H
#define SimpleScenegraph_H

#include  <GLES2/gl2.h>
#include  <GLES2/gl2ext.h>
#include  <EGL/egl.h>

#include <string>
#include <vector>

#include "GLWrapper.h"
#include "SimpleMesh.h"
#include "SimpleTexture.h"
#include "SimpleShader.h"

// Class definition

class SimpleScenegraph
{
public:
    SimpleScenegraph();
    ~SimpleScenegraph();

public:
    bool initScenegraph(unsigned int w, unsigned int h);
    void setCameraLocation(GLfloat x, GLfloat y, GLfloat z);
    void setCameraDelta(GLfloat x, GLfloat y, GLfloat z);

    /* File I/O */
    bool fromFile(const char *filename);
    void render(void);

protected:

private:
    SimpleShader *ss;
    std::vector <SimpleMesh *> v_sm;
    //std::vector <SimpleTexture *> v_st;

    // Matrix uniform and camera coords
    GLfloat camera_x, camera_y, camera_z;
    unsigned int w_width, w_height;
};

#endif // SimpleScenegraph_H

