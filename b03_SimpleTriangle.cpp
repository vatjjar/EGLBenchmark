
/*
 * b03_SimpleTriangle - benchmark #3, Simple triangle drawing scene
 * ------------------------------------------------------------------
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 *
 */

#include "b03_SimpleTriangle.h"

#include "DebugLog.h"

/*
 * Constructor and destructor are dummy ones. Only descriptions are set, and other activities are
 * done when calling the virtual benchmark API
 */

b03_SimpleTriangle::b03_SimpleTriangle() :
    renderedFrames(0),
    vertexShader(0),
    fragmentShader(0),
    shaderProgram(0)
{
    setName("GLES2 Simple Triangle drawing test");
    setDescription("This test renders a simple triangle scene");
}

b03_SimpleTriangle::~b03_SimpleTriangle()
{
}

/*
 * initBenchmark() shall initialize all required resources for this test case. If initialization fails,
 * false must be returned to indicate core benchmark not to continue execution. DebugLog::Instance()->MESSAGE()
 * method can be used to output information about the initialization
 */
bool b03_SimpleTriangle::initBenchmark(unsigned int width, unsigned int height, bool fullscreen)
{
    const char vertex_src[] =
       "attribute vec4 vPosition;    \n"
       "void main()                  \n"
       "{                            \n"
       "   gl_Position = vPosition;  \n"
       "}                            \n";

    const char fragment_src[] =
       "precision mediump float;\n"\
       "void main()                                  \n"
       "{                                            \n"
       "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
       "}                                            \n";

    if (false == createEGLDisplay(width, height, fullscreen))
        return false;

    shaderProgram = createShaderProgram(vertex_src, fragment_src);
    if (shaderProgram == 0)
    {
        DebugLog::Instance()->MESSAGE(1, "Error: Shader program object creation failed\n");
    }
    glwrap->GLBINDATTRIBLOCATION(shaderProgram, 0, "vPosition");
    glwrap->GLLINKPROGRAM(shaderProgram);

    return true;
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b03_SimpleTriangle::destroyBenchmark(void)
{
    destroyEGLDisplay();
    return true;
}


static GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                               -0.5f, -0.5f, 0.0f,
                                0.5f, -0.5f, 0.0f };

void b03_SimpleTriangle::Render(void)
{
    glwrap->GLVIEWPORT(0, 0, w_width, w_height);
    glwrap->GLCLEAR(GL_COLOR_BUFFER_BIT);
    glwrap->GLUSEPROGRAM(shaderProgram);
    glwrap->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glwrap->GLENABLEVERTEXATTRIBARRAY(0);
    glwrap->GLDRAWARRAYS(GL_TRIANGLES, 0, 3);

    glwrap->EGLSWAPBUFFERS(egl_display, egl_surface);
}


/*
 * renderSingleFrame()
 */
bool b03_SimpleTriangle::renderSingleFrame(float deltatime)
{
    Render();
    return true;
}
