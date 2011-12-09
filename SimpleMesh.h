
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

// Class definition

class SimpleMesh
{
public:
    SimpleMesh();
    ~SimpleMesh();

public:
    /* File I/O */
    bool fromFiles(const char *filename);

    /* Rendering methods: */
    void renderAsIndexedElements(void);
    void renderAsIndexedElements_VBO(void);

    void renderAsArrays(void);
    void renderAsArrays_VBO(void);

    bool createArrayData(void);
protected:

private:
    bool fromFileToFloatVector(const char *filename, GLuint *n_elements, GLfloat **a_elements);
    bool fromFileToShortVector(const char *filename, GLuint *n_elements, GLshort **a_elements);

    GLuint n_vertices;
    GLuint n_normals;
    GLuint n_texcoords;
    GLuint n_faces;
    GLfloat *a_vertices;
    GLfloat *a_normals;
    GLfloat *a_texcoords;
    GLshort *a_faces;

    // Array data (non-indexed)
    GLfloat *a_array_vertices;
    GLfloat *a_array_normals;
    GLfloat *a_array_texcoords;
};

#endif // SIMPLEMESH_H

