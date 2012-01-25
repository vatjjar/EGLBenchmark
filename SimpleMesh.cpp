
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "SimpleMesh.h"
#include "GLWrapper.h"
#include "DebugLog.h"
#include "GLMath.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>

SimpleMesh::SimpleMesh() :
    n_vertices(0),
    n_normals(0),
    n_texcoords(0),
    n_faces(0),
    a_vertices(NULL),
    a_normals(NULL),
    a_texcoords(NULL),
    a_faces(NULL),
    loc_x(0),
    loc_y(0),
    loc_z(0),
    shader(NULL),
    a_array_vertices(NULL),
    a_array_normals(NULL),
    a_array_texcoords(NULL),
    r_vertices(0),
    r_normals(0),
    r_texcoords(0),
    r_batches(0)
{
    memset(VBOs, 0, sizeof(VBOs));
}

SimpleMesh::~SimpleMesh()
{
}

/******************************************************************************
 * File I/O
 */

bool SimpleMesh::fromFiles(const char *filename)
{
    char fn[64];

    memset(fn, 0, sizeof(fn));
    memcpy(fn, filename, strlen(filename));

    // Faces (indices):
    memcpy(&fn[strlen(filename)], ".faces", strlen(".faces"));
    if (false == fromFileToShortVector(fn, &n_faces, &a_faces)) return false;
    if (n_faces%3 != 0) return false;
    n_faces /= 3;

    // Normals:
    memcpy(&fn[strlen(filename)], ".normals", strlen(".normals"));
    if (false == fromFileToFloatVector(fn, &n_normals, &a_normals)) return false;
    if (n_normals%3 != 0) return false;
    n_normals /= 3;

    // Vertices:
    memcpy(&fn[strlen(filename)], ".vertices", strlen(".vertices"));
    if (false == fromFileToFloatVector(fn, &n_vertices, &a_vertices)) return false;
    if (n_vertices%3 != 0) return false;
    n_vertices /= 3;

    // Texcoords:
    memcpy(&fn[strlen(filename)], ".texcoords", strlen(".texcoords"));
    if (false == fromFileToFloatVector(fn, &n_texcoords, &a_texcoords)) return false;
    if (n_texcoords%2 != 0) return false;
    n_texcoords /= 2;

    DebugLog::Instance()->MESSAGE(2, "Loaded mesh %s with %d vertices, %d normals, %d texcoords and %d faces\n",
         filename, n_vertices, n_normals, n_texcoords, n_faces);

    // Input data for the simple mesh is structured as an element list:
    // - Each vertex and  per-vertex data is specified only once
    // - a separate index buffer is provided which mark rendering indices

    // Using this data we convert a copy to arrays, and drop indices
    if (false == createArrayData())
    {
        DebugLog::Instance()->MESSAGE(2, "Simplemesh: array creation failed\n");
        return false;
    }

    return true;
}

void SimpleMesh::setShader(SimpleShader &s)
{
    shader = s;
}

void SimpleMesh::setLocation(GLfloat x, GLfloat y, GLfloat z)
{
    loc_x = x;
    loc_y = y;
    loc_z = z;
}

void SimpleMesh::getModelview(Matrix4X4 *m)
{
    GLMath::Instance()->_glLoadIdentity(m);
    GLMath::Instance()->_glTranslate(m, loc_x, loc_y, loc_z);
}

/******************************************************************************
 * Render methods
 */

void SimpleMesh::renderAsIndexedElements(void)
{
    if (shader == NULL)
    {
        DebugLog::Instance()->MESSAGE(5, "No shader set for the mesh. Render skipped\n");
        return
    }
    shader->bindProgram();

    // Assumption is that Vertex attrib array are ordered as follows:
    // 0: for vertices
    // 1: for texcoords
    // 2: for indices
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, a_vertices);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(0);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(1, 2, GL_FLOAT, GL_FALSE, 0, a_texcoords);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(1);

    GLWrapper::Instance()->GLDRAWELEMENTS(GL_TRIANGLES, 3*n_faces, GL_UNSIGNED_SHORT, a_faces);

    r_vertices += n_vertices;
    r_texcoords += n_texcoords;
    r_faces += n_faces;
    r_batches += 1;
}

void SimpleMesh::renderAsIndexedElements_VBO(void)
{
    if (shader == NULL)
    {
        DebugLog::Instance()->MESSAGE(5, "No shader set for the mesh. Render skipped\n");
        return
    }
    shader->bindProgram();

    // Assumption is that Vertex attrib array are ordered as follows:
    // 0: for vertices
    // 1: for texcoords
    // 2: for indices

    if (VBOs[0] == 0) // Initial creation of VBOs
    {
        GLWrapper::Instance()->GLGENBUFFERS(3, VBOs);
        if (VBOs[0] == 0)
        {
            DebugLog::Instance()->MESSAGE(2, "SimpleMesh: VBO creation failed\n");
            return;
        }
        GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, VBOs[0]);
        GLWrapper::Instance()->GLBUFFERDATA(GL_ARRAY_BUFFER, n_vertices*3*sizeof(GLfloat), a_vertices, GL_STATIC_DRAW);

        GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, VBOs[1]);
        GLWrapper::Instance()->GLBUFFERDATA(GL_ARRAY_BUFFER, n_texcoords*2*sizeof(GLfloat), a_texcoords, GL_STATIC_DRAW);

        GLWrapper::Instance()->GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER, VBOs[2]);
        GLWrapper::Instance()->GLBUFFERDATA(GL_ELEMENT_ARRAY_BUFFER, n_faces*3*sizeof(GLshort), a_faces, GL_STATIC_DRAW);

        // At this point it should be safe to delete original data, since it is in GPU mem
    }

    // Assumption is that Vertex attrib array are ordered as follows:
    // 0: for vertices
    GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, VBOs[0]);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(0);

    // 1: for texcoords
    GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, VBOs[1]);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(1);

    // 2: for indices
    GLWrapper::Instance()->GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER, VBOs[2]);
    GLWrapper::Instance()->GLDRAWELEMENTS(GL_TRIANGLES, 3*n_faces, GL_UNSIGNED_SHORT, NULL);

    // Generally a good idea to disable VBOs after rendering is done
    GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, 0);
    GLWrapper::Instance()->GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER, 0);

    r_vertices += n_vertices;
    r_texcoords += n_texcoords;
    r_faces += n_faces;
    r_batches += 1;
}

void SimpleMesh::renderAsArrays(void)
{
    if (shader == NULL)
    {
        DebugLog::Instance()->MESSAGE(5, "No shader set for the mesh. Render skipped\n");
        return
    }
    shader->bindProgram();

    // Assumption is that Vertex attrib array are ordered as follows:
    // 0: for vertices
    // 1: for texcoords
    // 2: for indices
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, a_array_vertices);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(0);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(1, 2, GL_FLOAT, GL_FALSE, 0, a_array_texcoords);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(1);

    GLWrapper::Instance()->GLDRAWARRAYS(GL_TRIANGLES, 0, 3*n_faces);

    r_vertices += 3*n_faces;
    r_texcoords += 3*n_faces;
    r_faces += n_faces;
    r_batches += 1;
}

void SimpleMesh::renderAsArrays_VBO(void)
{
    if (shader == NULL)
    {
        DebugLog::Instance()->MESSAGE(5, "No shader set for the mesh. Render skipped\n");
        return
    }
    shader->bindProgram();

    // Assumption is that Vertex attrib array are ordered as follows:
    // 0: for vertices
    // 1: for texcoords
    // 2: for indices (not used for rendering arrays)

    if (VBOs[0] == 0) // Initial creation of VBOs
    {
        GLWrapper::Instance()->GLGENBUFFERS(2, VBOs);
        if (VBOs[0] == 0)
        {
            DebugLog::Instance()->MESSAGE(2, "SimpleMesh: VBO creation failed\n");
            return;
        }
        GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, VBOs[0]);
        GLWrapper::Instance()->GLBUFFERDATA(GL_ARRAY_BUFFER, 9*n_faces*sizeof(GLfloat), a_array_vertices, GL_STATIC_DRAW);

        GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, VBOs[1]);
        GLWrapper::Instance()->GLBUFFERDATA(GL_ARRAY_BUFFER, 6*n_faces*sizeof(GLfloat), a_array_texcoords, GL_STATIC_DRAW);

        // At this point it should be safe to delete original data, since it is in GPU mem
        // however we don't.. for further tests later
    }

    // Assumption is that Vertex attrib array are ordered as follows:
    // 0: for vertices
    GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, VBOs[0]);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(0);

    // 1: for texcoords
    GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, VBOs[1]);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(1);

    // Draw arrays:
    GLWrapper::Instance()->GLDRAWARRAYS(GL_TRIANGLES, 0, 3*n_faces);

    // Generally a good idea to disable VBOs after rendering is done
    GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, 0);
    //GLWrapper::Instance()->GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER, 0);

    r_vertices += 3*n_faces;
    r_texcoords += 3*n_faces;
    r_faces += n_faces;
    r_batches += 1;
}

/******************************************************************************
 * Render statistics and their control
 */

// Rendering stats
unsigned int SimpleMesh::getRenderedVertices(void)
{
    return r_vertices;
}

unsigned int SimpleMesh::getRenderedNormals(void)
{
    return r_normals;
}

unsigned int SimpleMesh::getRenderedTexcoords(void)
{
    return r_texcoords;
}

unsigned int SimpleMesh::getRenderedFaces(void)
{
    return r_faces;
}

unsigned int SimpleMesh::getRenderedBatches(void)
{
    return r_batches;
}

void SimpleMesh::resetRenderingStatistics(void)
{
    r_vertices = 0;
    r_normals = 0;
    r_texcoords = 0;
    r_faces = 0;
    r_batches = 0;
}


/******************************************************************************
 * Private:
 */


bool SimpleMesh::createArrayData(void)
{
    // indexed data at this point in
    // - a_vertices
    // - a_normals
    // - a_texcoords
    // - a_faces

    if (a_array_vertices != NULL) delete a_array_vertices;
    a_array_vertices = new GLfloat [3*n_faces*3];
    if (a_array_normals != NULL) delete a_array_normals;
    a_array_normals = new GLfloat [3*n_faces*3];
    if (a_array_texcoords != NULL) delete a_array_texcoords;
    a_array_texcoords = new GLfloat [3*n_faces*2];

    DebugLog::Instance()->MESSAGE(2, "Building array from loaded mesh: %d vertices, %d normals, %d texcoords, %d faces\n",
        9*n_faces, 9*n_faces, 6*n_faces, n_faces);

    for (unsigned int i=0; i<n_faces; i++)
    {
        // vertex data for face corner #1
        a_array_vertices[9*i+0] = a_vertices[3*a_faces[3*i+0]+0];
        a_array_vertices[9*i+1] = a_vertices[3*a_faces[3*i+0]+1];
        a_array_vertices[9*i+2] = a_vertices[3*a_faces[3*i+0]+2];
        // face corner #2
        a_array_vertices[9*i+3] = a_vertices[3*a_faces[3*i+1]+0];
        a_array_vertices[9*i+4] = a_vertices[3*a_faces[3*i+1]+1];
        a_array_vertices[9*i+5] = a_vertices[3*a_faces[3*i+1]+2];
        // face corner #3
        a_array_vertices[9*i+6] = a_vertices[3*a_faces[3*i+2]+0];
        a_array_vertices[9*i+7] = a_vertices[3*a_faces[3*i+2]+1];
        a_array_vertices[9*i+8] = a_vertices[3*a_faces[3*i+2]+2];

        // normal data for face corner #1
        a_array_normals[9*i+0] = a_normals[3*a_faces[3*i+0]+0];
        a_array_normals[9*i+1] = a_normals[3*a_faces[3*i+0]+1];
        a_array_normals[9*i+2] = a_normals[3*a_faces[3*i+0]+2];
        // face corner #2
        a_array_normals[9*i+3] = a_normals[3*a_faces[3*i+1]+0];
        a_array_normals[9*i+4] = a_normals[3*a_faces[3*i+1]+1];
        a_array_normals[9*i+5] = a_normals[3*a_faces[3*i+1]+2];
        // face corner #3
        a_array_normals[9*i+6] = a_normals[3*a_faces[3*i+2]+0];
        a_array_normals[9*i+7] = a_normals[3*a_faces[3*i+2]+1];
        a_array_normals[9*i+8] = a_normals[3*a_faces[3*i+2]+2];

        // texcoord data for face corner #1
        a_array_texcoords[6*i+0] = a_texcoords[2*a_faces[3*i+0]+0];
        a_array_texcoords[6*i+1] = a_texcoords[2*a_faces[3*i+0]+1];
        // face corner #2
        a_array_texcoords[6*i+2] = a_texcoords[2*a_faces[3*i+1]+0];
        a_array_texcoords[6*i+3] = a_texcoords[2*a_faces[3*i+1]+1];
        // face corner #3
        a_array_texcoords[6*i+4] = a_texcoords[2*a_faces[3*i+2]+0];
        a_array_texcoords[6*i+5] = a_texcoords[2*a_faces[3*i+2]+1];
    }
    return true;
}

/******************************************************************************
 * Private methods:
 */

bool SimpleMesh::fromFileToFloatVector(const char *filename, GLuint *n_elements, GLfloat **a_elements)
{
    FILE *f;
    char linebuffer[16];
    GLfloat *elements = new GLfloat[20480];
    unsigned int i;

    DebugLog::Instance()->MESSAGE(4, "SimpleMesh: trying to open file: %s\n", filename);
    f = fopen(filename, "r");
    if (f == NULL)
    {
        DebugLog::Instance()->MESSAGE(4, "File open failed\n");
        return false;
    }

    i = 0;
    while (fgets(linebuffer, sizeof(linebuffer), f) != NULL)
    {
        elements[i] = atof(linebuffer);
        DebugLog::Instance()->MESSAGE(5, "Read %d item from file: %f\n", i, elements[i]);
        i++;
        if (i>=20480) return false; // Guard
    }
    *a_elements = elements;
    *n_elements = i;
    return true;
}

bool SimpleMesh::fromFileToShortVector(const char *filename, GLuint *n_elements, GLshort **a_elements)
{
    FILE *f;
    char linebuffer[16];
    GLshort *elements = new GLshort[20480];
    unsigned int i;

    DebugLog::Instance()->MESSAGE(4, "SimpleMesh: trying to open file: %s\n", filename);
    f = fopen(filename, "r");
    if (f == NULL)
    {
        DebugLog::Instance()->MESSAGE(4, "File open failed\n");
        return false;
    }

    i = 0;
    while (fgets(linebuffer, sizeof(linebuffer), f) != NULL)
    {
        elements[i] = atoi(linebuffer);
        DebugLog::Instance()->MESSAGE(5, "Read %d item from file: %d\n", i, elements[i]);
        i++;
        if (i>=20480) return false; // Guard
    }
    *a_elements = elements;
    *n_elements = i;
    return true;
}
