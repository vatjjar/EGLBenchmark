
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef SIMPLEMESH_H
#define SIMPLEMESH_H

#include  <GLES2/gl2.h>
#include  <GLES2/gl2ext.h>
#include  <EGL/egl.h>

#include <string>

#include "GLWrapper.h"
#include "GLMath.h"
#include "SimpleShader.h"

// Class definition

class SimpleMesh
{
public:
    SimpleMesh();
    ~SimpleMesh();

public:
    /* File I/O */
    bool fromFiles(const char *filename);

    /* Params */
    void setShader(SimpleShader &shader);
    void setLocation(GLfloat x, GLfloat y, GLfloat z);
    void getModelview(Matrix4X4 *m);

    /* Rendering methods: */
    void renderAsIndexedElements(void);
    void renderAsIndexedElements_VBO(void);
    void renderAsArrays(void);
    void renderAsArrays_VBO(void);

    // Rendering stats
    unsigned int getRenderedVertices(void);
    unsigned int getRenderedNormals(void);
    unsigned int getRenderedTexcoords(void);
    unsigned int getRenderedFaces(void);
    unsigned int getRenderedBatches(void);
    void resetRenderingStatistics(void);

protected:

private:
    bool fromFileToFloatVector(const char *filename, GLuint *n_elements, GLfloat **a_elements);
    bool fromFileToShortVector(const char *filename, GLuint *n_elements, GLshort **a_elements);
    bool createArrayData(void);

    GLuint n_vertices;
    GLuint n_normals;
    GLuint n_texcoords;
    GLuint n_faces;
    GLfloat *a_vertices;
    GLfloat *a_normals;
    GLfloat *a_texcoords;
    GLshort *a_faces;

    // Location (simple placeable component)
    GLfloat loc_x, loc_y, loc_z;

    // Attached shader object
    SimpleShader &shader;

    // Array data (non-indexed, and generated runtime)
    GLfloat *a_array_vertices;
    GLfloat *a_array_normals;
    GLfloat *a_array_texcoords;

    // Render statistics
    unsigned int r_vertices;
    unsigned int r_normals;
    unsigned int r_texcoords;
    unsigned int r_faces;
    unsigned int r_batches;

    // VBO Handles
    GLuint VBOs[3];
};

#endif // SIMPLEMESH_H

