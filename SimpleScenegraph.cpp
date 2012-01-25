
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

    // Delete texture array
    for (i=0; i<v_st.size(); i++)
    {
        delete v_st[i];
    }
}

bool SimpleScenegraph::init(unsigned int w, unsigned int h)
{
    const char vertex_src[] =
       "attribute vec4 a_Position;       \n"
       "attribute vec2 a_Texcoord;       \n"
       "varying   vec2 v_Texcoord;       \n"
       "uniform   mat4 u_RotationMatrix; \n"
       "void main()                      \n"
       "{                                \n"
       "   gl_Position = u_RotationMatrix * a_Position; \n"
       "   v_Texcoord = a_Texcoord;      \n"
       "}                                \n";
    const char fragment_src[] =
       "precision mediump float;                     \n"
       "varying vec2 v_Texcoord;                     \n"
       "uniform sampler2D s_texture;                 \n"
       "void main()                                  \n"
       "{                                            \n"
       "  gl_FragColor = texture2D(s_texture, v_Texcoord);\n"
       "}                                            \n";

    w_width = w;
    w_height = h;

    ss = new SimpleShader();
    if (false == ss->fromFiles(vertex_src, fragment_src))
    {
        DebugLog::Instance()->MESSAGE(2, "Shader program object creation failed\n");
        return false;
    }
    GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 0, "a_Position");
    GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 1, "a_Texcoord");
    //GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 3, "u_RotationMatrix");
    ss->linkProgram();
    //a_position = GLWrapper::Instance()->GLATTRIBLOCATION(ss->getProgramObject(), "u_Position");
    //a_texcoord = GLWrapper::Instance()->GLATTRIBLOCATION(ss->getProgramObject(), "u_Texcoord");
    u_matrix = GLWrapper::Instance()->GLGETUNIFORMLOCATION(ss->getProgramObject(), "u_RotationMatrix");

    GLWrapper::Instance()->GLCLEARCOLOR(0, 0, 0, 0);
    GLWrapper::Instance()->GLVIEWPORT(0, 0, w_width, w_height);

    return true
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
        SimpleTexture *st;

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
        if (false == sm->fromFiles(mesh_ptr))
        {
            DebugLog::Instance()->MESSAGE(4, "Mesh load error!\n");
            fclose(f);
            delete sm;
            return false;
        }
        sm->setShader(ss); // Install a default shader for the mesh rendering
        v_sm.push_back(sm);

        // the process the texture:
        st = new SimpleTexture();
        if (false == st->fromFile(image_ptr))
        {
            DebugLog::Instance()->MESSAGE(4, "Image load error!\n");
            fclose(f);
            delete st;
            return false;
        }
        v_st.push_back(st);

        guard++;
        if (guard >= 100) return false; // Guarding limit to accept 100 mesh/tex pairs only for testing
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
    Matrix4x4 perspective;
    Matrix4x4 modelview;
    Matrix4x4 result;
    unsigned int i;

    GLWrapper::Instance()->GLVIEWPORT(0, 0, w_width, w_height);
    GLWrapper::Instance()->GLCLEAR(GL_COLOR_BUFFER_BIT);

    // Generate a perspective matrix with a 60 degree FOV
    GLMath::Instance()->_glLoadIdentity(&perspective);
    GLMath::Instance()->_glPerspective(&perspective, 45.0f, w_width/w_height, 1.0f, 20.0f);
    GLMath::Instance()->_glTranslate(&perspective, camera_x, camera_y, camera_z);

    for (i=0; i<v_st.size(); i++)
    {
        v_sm[i]->getModelview(&modelview);
        GLMath::Instance()->_glMatrixMultiply( &result, &modelview, &perspective );
        // Load the MVP matrix
        glUniformMatrix4fv( u_matrix, 1, GL_FALSE, (GLfloat*) &result );

        v_st[i]->bind();
        v_sm[i]->renderAsIndexedElements_VBO();
    }
}

/******************************************************************************
 * Private methods:
 */
