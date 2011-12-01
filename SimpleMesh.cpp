
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "SimpleMesh.h"
#include "GLWrapper.h"
#include "DebugLog.h"

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
    a_faces(NULL)
{
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
    fromFileToShortVector(fn, &n_faces, &a_faces);
    //std::cout << "Read file " << fn << " " << n_faces << " elements\n";
    if (n_vertices%3 != 0) return false;
    n_vertices /= 3;

    // Normals:
    memcpy(&fn[strlen(filename)], ".normals", strlen(".normals"));
    fromFileToFloatVector(fn, &n_normals, &a_normals);
    //std::cout << "Read file " << fn << " " << n_normals << " elements\n";
    if (n_normals%3 != 0) return false;
    n_normals /= 3;

    // Vertices:
    memcpy(&fn[strlen(filename)], ".vertices", strlen(".vertices"));
    fromFileToFloatVector(fn, &n_vertices, &a_vertices);
    //std::cout << "Read file " << fn << " " << n_vertices << " elements\n";
    if (n_vertices%3 != 0) return false;
    n_vertices /= 3;

    // Texcoords:
    memcpy(&fn[strlen(filename)], ".texcoords", strlen(".texcoords"));
    fromFileToFloatVector(fn, &n_texcoords, &a_texcoords);
    //std::cout << "Read file " << fn << " " << n_texcoords << " elements\n";
    if (n_texcoords%2 != 0) return false;
    n_texcoords /= 2;

    return true;
}

/******************************************************************************
 * Render
 */

void SimpleMesh::renderAsIndexedElements(void)
{
    // Assumption is that Vertex attrib array 0 is for vertices
    // and array 1 for texcoords
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, a_vertices);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(0);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(1, 2, GL_FLOAT, GL_FALSE, 0, a_texcoords);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(1);

    //std::cout << "rendering arrays " << 3*n_faces << "\n";
    GLWrapper::Instance()->GLDRAWELEMENTS(GL_TRIANGLES, 3*n_faces, GL_UNSIGNED_SHORT, a_faces);
    //glDrawArrays(GL_TRIANGLES, 0, n_vertices);
}

void SimpleMesh::renderAsIndexedElements_VBO(void)
{
    GLuint VBOs[3];

    GLWrapper::Instance()->GLGENBUFFERS(3, VBOs);

    GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, VBOs[0]);
    GLWrapper::Instance()->GLBUFFERDATA(GL_ARRAY_BUFFER, n_vertices*3*sizeof(GLfloat), a_vertices, GL_STATIC_DRAW);
    GLWrapper::Instance()->GLBINDBUFFER(GL_ARRAY_BUFFER, VBOs[1]);
    GLWrapper::Instance()->GLBUFFERDATA(GL_ARRAY_BUFFER, n_texcoords*2*sizeof(GLfloat), a_texcoords, GL_STATIC_DRAW);

    GLWrapper::Instance()->GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER, VBOs[2]);
    GLWrapper::Instance()->GLBUFFERDATA(GL_ELEMENT_ARRAY_BUFFER, n_faces*3*sizeof(GLshort), a_faces, GL_STATIC_DRAW);

    // Assumption is that Vertex attrib array 0 is for vertices
    // and array 1 for texcoords
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(0);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(1);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, a_vertices);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(1, 2, GL_FLOAT, GL_FALSE, 0, a_texcoords);

    GLWrapper::Instance()->GLDRAWELEMENTS(GL_TRIANGLES, 3*n_faces, GL_UNSIGNED_SHORT, a_faces);

    GLWrapper::Instance()->GLDELETEBUFFERS(3, VBOs);
}

void SimpleMesh::renderAsArrays(void)
{
}

void SimpleMesh::renderAsVertexbufferobject(void)
{

}

/******************************************************************************
 * Private methods:
 */

bool SimpleMesh::fromFileToFloatVector(const char *filename, GLuint *n_elements, GLfloat **a_elements)
{
    FILE *f;
    char linebuffer[16];
    GLfloat *elements = new GLfloat[1024];
    unsigned int i;

    DebugLog::Instance()->MESSAGE(4, "SimpleMesh: trying to open file: %s\n", filename);
    f = fopen(filename, "r");
    if (f == NULL) return false;

    i = 0;
    while (fgets(linebuffer, sizeof(linebuffer), f) != NULL)
    {
        elements[i] = atof(linebuffer);
        DebugLog::Instance()->MESSAGE(5, "Read item from file: %f\n", elements[i]);
        i++;
        if (i>=1024) return false; // Guard
    }
    *a_elements = elements;
    *n_elements = i;
    return true;
}

bool SimpleMesh::fromFileToShortVector(const char *filename, GLuint *n_elements, GLshort **a_elements)
{
    FILE *f;
    char linebuffer[16];
    GLshort *elements = new GLshort[1024];
    unsigned int i;

    DebugLog::Instance()->MESSAGE(4, "SimpleMesh: trying to open file: %s\n", filename);
    f = fopen(filename, "r");
    if (f == NULL) return false;

    i = 0;
    while (fgets(linebuffer, sizeof(linebuffer), f) != NULL)
    {
        elements[i] = atoi(linebuffer);
        DebugLog::Instance()->MESSAGE(5, "Read item from file: %d\n", elements[i]);
        i++;
        if (i>=1024) return false; // Guard
    }
    *a_elements = elements;
    *n_elements = i;
    return true;
}
