
/*
 * b02_SimpleGLShading - benchmark #2, Simple shading scene rendering
 * ------------------------------------------------------------------
 * This test case setups EGL context and draws a quad with a spiral
 * generated by simple shaders. Original source code is released in
 * Meamo forums (http://wiki.maemo.org/SimpleGL_example) and adapted
 * by me for this benchmark suite.
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 *
 * Credit to the original authors of the code:
 * (Created by exoticorn ( http://talk.maemo.org/showthread.php?t=37356 )
 * edited and commented by André Bergner [endboss])
 *
 */

#include "b02_SimpleGLShading.h"

/*
 * local constants for the test case:
 */

static const char vertex_src [] =
"                                        \
   attribute vec4        position;       \
   varying mediump vec2  pos;            \
   uniform vec4          offset;         \
                                         \
   void main()                           \
   {                                     \
      gl_Position = position + offset;   \
      pos = position.xy;                 \
   }                                     \
";

static const char fragment_src [] =
"                                                      \
   varying mediump vec2    pos;                        \
   uniform mediump float   phase;                      \
                                                       \
   void  main()                                        \
   {                                                   \
      gl_FragColor  =  vec4( 1., 0.9, 0.7, 1.0 ) *     \
        cos( 30.*sqrt(pos.x*pos.x + 1.5*pos.y*pos.y)   \
             + atan(pos.y,pos.x) - phase );            \
   }                                                   \
";

static const float vertexArray[] = {
     0.0,  0.5,  0.0,
    -0.5,  0.0,  0.0,
     0.0, -0.5,  0.0,
     0.5,  0.0,  0.0,
     0.0,  0.5,  0.0
 };

/*
 * Constructor and destructor are dummy ones. Only descriptions are set, and other activities are
 * done when calling the virtual benchmark API
 */

b02_SimpleGLShading::b02_SimpleGLShading() :
    renderedFrames(0),
    phase(0),
    vertexShader(0),
    fragmentShader(0),
    shaderProgram(0),
    phase_loc(0),
    offset_loc(0),
    position_loc(0)
{
    setName("GLES2 Shading test");
    setDescription("This test renders a simple shading scene");
}

b02_SimpleGLShading::~b02_SimpleGLShading()
{
}

/*
 * initBenchmark() shall initialize all required resources for this test case. If initialization fails,
 * false must be returned to indicate core benchmark not to continue execution. Parent class outputMessage()
 * method can be used to output information about the initialization
 */
bool b02_SimpleGLShading::initBenchmark(unsigned int width, unsigned int height, bool fullscreen)
{
    std::cout << "lal\n";
    return createEGLDisplay(width, height, fullscreen);
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b02_SimpleGLShading::destroyBenchmark(void)
{
    destroyEGLDisplay();
    return true;
}


void b02_SimpleGLShading::Render(void)
{
    glClear ( GL_COLOR_BUFFER_BIT );
    flushGLErrors();

    glUniform1f ( phase_loc , phase );
    flushGLErrors();

    phase  =  phase+0.5f;

    glUniform4f ( offset_loc  ,  0.0 , 0.0 , 0.0 , 0.0 );
    flushGLErrors();

    glVertexAttribPointer ( position_loc, 3, GL_FLOAT, false, 0, vertexArray );
    flushGLErrors();

    glEnableVertexAttribArray ( position_loc );
    flushGLErrors();

    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 5 );
    flushGLErrors();

    eglSwapBuffers ( egl_display, egl_surface );  // get the rendered buffer to the screen
    flushGLErrors();
}


/*
 * runBenchmark()
 */
bool b02_SimpleGLShading::runBenchmark(float duration)
{
 //   XWindowAttributes  gwa;

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

    outputMessage(5, "glLinkProgram()\n");
    glLinkProgram ( shaderProgram );
    flushGLErrors();
    outputMessage(5, "glUseProgram()\n");
    glUseProgram  ( shaderProgram );
    flushGLErrors();

    outputMessage(5, "glGetAttribLocation() 1\n");
    position_loc  = glGetAttribLocation  ( shaderProgram , "position" );
    flushGLErrors();
    outputMessage(5, "glGetAttribLocation() 2\n");
    phase_loc     = glGetUniformLocation ( shaderProgram , "phase"    );
    flushGLErrors();
    outputMessage(5, "glGetAttribLocation() 3\n");
    offset_loc     = glGetUniformLocation ( shaderProgram , "offset"    );
    flushGLErrors();

    if ( position_loc < 0  ||  phase_loc < 0  ||  offset_loc < 0 ) {
       outputMessage(1, "Error: Unable to get uniform location\n");
       return false;
    }

    outputMessage(5, "glViewport\n");
    glViewport ( 0 , 0 , w_width , w_height );
    flushGLErrors();
    outputMessage(5, "glClearColor\n");
    glClearColor ( 0.08 , 0.06 , 0.07 , 1.);
    flushGLErrors();

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
bool b02_SimpleGLShading::displayResult(void)
{
    MESSAGE(1, "Frames per second %f\n", renderedFrames/totaltime);
    return false;
}
