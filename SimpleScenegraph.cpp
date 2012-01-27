
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "SimpleScenegraph.h"
#include "GLWrapper.h"
#include "DebugLog.h"
#include "GLMath.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>

SimpleScenegraph::SimpleScenegraph()
{
}

SimpleScenegraph::~SimpleScenegraph()
{
    unsigned int i;

    // Delete mesh array
    for (i=0; i<v_sm.size(); i++)
    {
        delete v_sm[i];
    }
}

bool SimpleScenegraph::initScenegraph(unsigned int w, unsigned int h)
{
    w_width = w;
    w_height = h;

    GLWrapper::Instance()->GLCLEARCOLOR(0, 0, 0, 0);
    GLWrapper::Instance()->GLVIEWPORT(0, 0, w_width, w_height);
    GLWrapper::Instance()->GLENABLE(GL_DEPTH_TEST);
    GLWrapper::Instance()->GLENABLE(GL_CULL_FACE);
    GLWrapper::Instance()->GLENABLE(GL_BLEND);
    GLWrapper::Instance()->GLBLENDFUNC(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void SimpleScenegraph::setCameraLocation(GLfloat x, GLfloat y, GLfloat z)
{
    camera_x = x;
    camera_y = y;
    camera_z = z;
}

void SimpleScenegraph::setCameraDelta(GLfloat x, GLfloat y, GLfloat z)
{
    camera_x += x;
    camera_y += y;
    camera_z += z;
}

/******************************************************************************
 * File I/O
 */

bool SimpleScenegraph::fromFile(const char *filename)
{
    FILE *f;
    char linebuffer[256];
    unsigned int guard;

    DebugLog::Instance()->MESSAGE(4, "SimpleScenegraph: trying to open file: %s\n", filename);
    f = fopen(filename, "r");
    if (f == NULL)
    {
        DebugLog::Instance()->MESSAGE(4, "File open failed\n");
        return false;
    }

    guard = 0;
    while (fgets(linebuffer, sizeof(linebuffer), f) != NULL)
    {
        char *image_ptr;
        char *mesh_ptr;
        SimpleMesh *sm;

        // input line is split between mesh and image ref, separated by ;
        // So first a littlebit of manipulation that the files can be opened
        linebuffer[strlen(linebuffer)-1] = '\0';
        mesh_ptr = &linebuffer[0];
        image_ptr = strstr(linebuffer, ";");
        *image_ptr++ = '\0';

        DebugLog::Instance()->MESSAGE(4, "Trying to load mesh %s ... and\n", mesh_ptr);
        DebugLog::Instance()->MESSAGE(4, "image file: %s\n", image_ptr);

        // First process the mesh
        sm = new SimpleMesh();
        if (false == sm->fromFiles(mesh_ptr) || false == sm->attachDefaultShader() || false == sm->attachDefaultTexture(image_ptr))
        {
            DebugLog::Instance()->MESSAGE(4, "Setting up scenegraph mesh failed!\n");
            fclose(f);
            delete sm;
            return false;
        }
        sm->setLocation((float)(rand()%16-8), 0.0f, (float)(rand()%16-8)); // artificial distribution of the meshes

        v_sm.push_back(sm); // Finally push mesh entity into scenegraph stack.

        guard++;
        if (guard >= 200) return false; // Guarding limit to accept 100 mesh/tex pairs only for testing
        DebugLog::Instance()->MESSAGE(3, "guard: %d\n", guard);
    }
    fclose(f);
    DebugLog::Instance()->MESSAGE(5, "Found total of %d entities\n", guard);
    return true;
}

/******************************************************************************
 * Render methods
 */

void SimpleScenegraph::render(void)
{
    Matrix4X4 perspective;
    Matrix4X4 modelview;
    Matrix4X4 result;
    unsigned int i;

    GLWrapper::Instance()->GLVIEWPORT(0, 0, w_width, w_height);
    GLWrapper::Instance()->GLCLEAR(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Generate a perspective matrix with a 60 degree FOV
    GLMath::Instance()->_glLoadIdentity(&perspective);
    GLMath::Instance()->_glPerspective(&perspective, 45.0f, w_width/w_height, 1.0f, 100.0f);
    GLMath::Instance()->_glTranslate(&perspective, camera_x, camera_y, camera_z);

    for (i=0; i<v_sm.size(); i++) // Loop through all objects
    {
        // Grab objects local modelview matrix
        v_sm[i]->getModelview(&modelview);
        // Apply the effect of camera
        GLMath::Instance()->_glMatrixMultiply( &result, &modelview, &perspective );
        // And pass the fully applied matrix to individual object render
        v_sm[i]->applyModelview(&result);

        v_sm[i]->renderAsIndexedElements_VBO();
    }
}

/******************************************************************************
 * Private methods:
 */
