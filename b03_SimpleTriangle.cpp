
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
#include "GLWrapper.h"

/*
 * Constructor and destructor are dummy ones. Only descriptions are set, and other activities are
 * done when calling the virtual benchmark API
 */

b03_SimpleTriangle::b03_SimpleTriangle() :
    renderedFrames(0)
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
bool b03_SimpleTriangle::initBenchmark(void)
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

    ss = new SimpleShader();
    if (false == ss->fromFiles(vertex_src, fragment_src))
    {
        DebugLog::Instance()->MESSAGE(2, "Shader program object creation failed\n");
        return false;
    }
    GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 0, "vPosition");
    ss->linkProgram();

    // If we have errors in GL pipe, then abort.
    if (GLWrapper::Instance()->getGLErrors() > 0) return false;

    return true;
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b03_SimpleTriangle::destroyBenchmark(void)
{
    return true;
}


static GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                               -0.5f, -0.5f, 0.0f,
                                0.5f, -0.5f, 0.0f };

void b03_SimpleTriangle::Render(void)
{
    GLWrapper::Instance()->GLVIEWPORT(0, 0, display->getDisplayWidth(), display->getDisplayHeight());
    GLWrapper::Instance()->GLCLEAR(GL_COLOR_BUFFER_BIT);
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(0);
    GLWrapper::Instance()->GLDRAWARRAYS(GL_TRIANGLES, 0, 3);

    // get the rendered buffer to the screen
    GLWrapper::Instance()->EGLSWAPBUFFERS(display->getEGLDisplay(), display->getEGLSurface());
}

/*
 * renderSingleFrame()
 */
bool b03_SimpleTriangle::renderSingleFrame(float deltatime)
{
    Render();
    return true;
}

/*
 * getRenderStatistics()
 */
bool b03_SimpleTriangle::getRenderStatistics(RENDER_STATISTICS *rs)
{
    return true;
}
