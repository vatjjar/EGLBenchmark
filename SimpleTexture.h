
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef SimpleTexture_H
#define SimpleTexture_H

#include  <GLES2/gl2.h>
#include  <GLES2/gl2ext.h>
#include  <EGL/egl.h>

#include <string>

#include "GLWrapper.h"

// Class definition

class SimpleTexture
{
public:
    SimpleTexture();
    ~SimpleTexture();

public:
    /* File I/O */
    bool fromFile(const char *filename);
    unsigned char * readBinaryFile(const char *filename, unsigned int & length);
    void bind(void);
    void destroy(void);

protected:
    GLuint loadETCTextureFromFile(const char *filename);
    GLuint loadRGBTexturefromPNG(const char *filename);

private:
    GLuint texture_id;
};

#endif // SimpleTexture_H
