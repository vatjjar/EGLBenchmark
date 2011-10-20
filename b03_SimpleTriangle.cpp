
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
 * false must be returned to indicate core benchmark not to continue execution. Parent class outputMessage()
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

    outputMessage(5, "loading shaders...\n");
    vertexShader   = loadShader ( vertex_src , GL_VERTEX_SHADER  );     // load vertex shader
    fragmentShader = loadShader ( fragment_src , GL_FRAGMENT_SHADER );  // load fragment shader
    if (vertexShader == 0 || fragmentShader == 0)
    {
        outputMessage(1, "Error: Shader program loading failed\n");
        return false;
    }

    outputMessage(5, "glCreateProgram()\n");
    shaderProgram  = glCreateProgram ();
    flushGLErrors();
    outputMessage(5, "glAttachShader() - vertex\n");
    glAttachShader ( shaderProgram, vertexShader );
    flushGLErrors();
    outputMessage(5, "glAttachShader() - fragment\n");
    glAttachShader ( shaderProgram, fragmentShader );
    flushGLErrors();

    glBindAttribLocation ( shaderProgram, 0, "vPosition" );
    flushGLErrors();

    outputMessage(5, "glLinkProgram()\n");
    glLinkProgram ( shaderProgram );
    flushGLErrors();
    outputMessage(5, "glUseProgram()\n");
    glUseProgram  ( shaderProgram );
    flushGLErrors();

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
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


void b03_SimpleTriangle::Render(void)
{
    GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                            -0.5f, -0.5f, 0.0f,
                             0.5f, -0.5f, 0.0f };

    // Set the viewport
    glViewport ( 0, 0, w_width, w_height);
    flushGLErrors();

    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );
    flushGLErrors();

    // Use the program object
    glUseProgram ( shaderProgram );
    flushGLErrors();

    // Load the vertex data
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    flushGLErrors();

    glEnableVertexAttribArray ( 0 );
    flushGLErrors();

    glDrawArrays ( GL_TRIANGLES, 0, 3 );
    flushGLErrors();

    eglSwapBuffers ( egl_display, egl_surface );  // get the rendered buffer to the screen
    flushGLErrors();
}


/*
 * runBenchmark()
 */
bool b03_SimpleTriangle::runBenchmark(float duration)
{
    // Timer and variables
    resetTimer();
    totaltime = 0;
    renderedFrames = 0;

    while ( totaltime < duration )
    {
        Render();
        renderedFrames++;

        // Grab time since last timer reset
        totaltime = getTime();

        if (userInterrupt() == true)
            break;
    }
    return true;
}

/*
 * displayResult()
 */
bool b03_SimpleTriangle::displayResult(void)
{
    MESSAGE(1, "Frames per second %f\n", renderedFrames/totaltime);
    return false;
}
