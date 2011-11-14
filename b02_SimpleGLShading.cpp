
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
 * false must be returned to indicate core benchmark not to continue execution. Parent class MESSAGE()
 * method can be used to output information about the initialization
 */
bool b02_SimpleGLShading::initBenchmark(unsigned int width, unsigned int height, bool fullscreen)
{
    if (false == createEGLDisplay(width, height, fullscreen))
    {
        return false;
    }

    shaderProgram = createShaderProgram(vertex_src, fragment_src);
    if (shaderProgram == 0)
    {
        MESSAGE(1, "Error: Shader program object creation failed\n");
    }
    linkShaderProgram(shaderProgram);

    position_loc  = GLGETATTRIBLOCATION(shaderProgram , "position");
    phase_loc     = GLGETUNIFORMLOCATION(shaderProgram , "phase"   );
    offset_loc    = GLGETUNIFORMLOCATION(shaderProgram , "offset"  );

    if ( position_loc < 0  ||  phase_loc < 0  ||  offset_loc < 0 ) {
       MESSAGE(1, "Error: Unable to get uniform location\n");
       return false;
    }

    GLVIEWPORT ( 0 , 0 , w_width , w_height );
    GLCLEARCOLOR(0.08 , 0.06 , 0.07 , 1.);

    // If we have errors in GL pipe, then abort.
    if (getGLErrors() > 0) return false;
    return true;
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
    GLCLEAR ( GL_COLOR_BUFFER_BIT );

    GLUNIFORM1F ( phase_loc , phase );

    phase  =  phase+0.5f;

    GLUNIFORM4F ( offset_loc  ,  0.0 , 0.0 , 0.0 , 0.0 );
    GLVERTEXATTRIBPOINTER ( position_loc, 3, GL_FLOAT, false, 0, vertexArray );
    GLENABLEVERTEXATTRIBARRAY ( position_loc );
    GLDRAWARRAYS ( GL_TRIANGLE_STRIP, 0, 5 );

    EGLSWAPBUFFERS ( egl_display, egl_surface );  // get the rendered buffer to the screen
}

/*
 * renderSingleFrame()
 */
bool b02_SimpleGLShading::renderSingleFrame(float deltatime)
{
    Render();
    return true;
}
