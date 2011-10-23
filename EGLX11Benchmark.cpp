
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "EGLX11Benchmark.h"

EGLX11Benchmark::EGLX11Benchmark() :
    name("No name"),
    description("No description"),
    win(0),
    x_display(NULL),
    egl_display(0),
    egl_surface(0),
    egl_context(0),
    verbosity(1),
    GLerrors(0),
    EGLerrors(0)
{
    // dummy constructor
}

EGLX11Benchmark::~EGLX11Benchmark()
{
}

const char * EGLX11Benchmark::getName(void)
{
    return name;
}

const char * EGLX11Benchmark::getDescription(void)
{
    return description;
}

void EGLX11Benchmark::setName(const char *n)
{
    name = n;
}

void EGLX11Benchmark::setDescription(const char *d)
{
    description = d;
}

void EGLX11Benchmark::outputMessage(int level, const char *message)
{
    if (verbosity >= level)
    {
        std::cout << level <<": " << message;
    }
}

void EGLX11Benchmark::flushGLErrors(void)
{
    GLuint error;
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        GLerrors++;
        std::cout << "GL ERROR: glGetError() returned " << error;
        switch(error)
        {
        case GL_INVALID_VALUE:
            std::cout << " (GL_INVALID_VALUE)\n";
            break;
        case GL_INVALID_ENUM:
            std::cout << " (GL_INVALID_ENUM)\n";
            break;
        case GL_INVALID_OPERATION:
            std::cout << " (GL_INVALID_OPERATION)\n";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << " (GL_INVALID_FRAMEBUFFER_OPERATION) ";
            std::cout << "glCheckFramebufferStatus(GL_FRAMEBUFFER) = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n";
            break;
        default:
            std::cout << " (UNKNOWN GL ERROR)\n";
            break;
        }
    }
}

void EGLX11Benchmark::flushEGLErrors(void)
{
    GLuint error;
    error = eglGetError();
    if (error != EGL_SUCCESS)
    {
        EGLerrors++;
        std::cout << "EGL ERROR: eglGetError() returned " << error;
        switch(error)
        {
        case EGL_NOT_INITIALIZED:
            std::cout << " (EGL_NOT_INITIALIZED)\n";
            break;
        case EGL_BAD_ACCESS:
            std::cout << " (EGL_BAD_ACCESS)\n";
            break;
        case EGL_BAD_ALLOC:
            std::cout << " (EGL_BAD_ALLOC)\n";
            break;
        case EGL_BAD_ATTRIBUTE:
            std::cout << " (EGL_BAD_ATTRIBUTE) ";
            break;
        case EGL_BAD_CONTEXT:
            std::cout << " (EGL_BAD_CONTEXT) ";
            break;
        case EGL_BAD_CONFIG:
            std::cout << " (EGL_BAD_CONFIG) ";
            break;
        case EGL_BAD_CURRENT_SURFACE:
            std::cout << " (EGL_BAD_CURRENT_SURFACE) ";
            break;
        case EGL_BAD_DISPLAY:
            std::cout << " (EGL_BAD_DISPLAY) ";
            break;
        case EGL_BAD_SURFACE:
            std::cout << " (EGL_BAD_SURFACE) ";
            break;
        case EGL_BAD_MATCH:
            std::cout << " (EGL_BAD_MATCH) ";
            break;
        case EGL_BAD_PARAMETER:
            std::cout << " (EGL_BAD_PARAMETER) ";
            break;
        case EGL_BAD_NATIVE_PIXMAP:
            std::cout << " (EGL_BAD_NATIVE_PIXMAP) ";
            break;
        case EGL_BAD_NATIVE_WINDOW:
            std::cout << " (EGL_BAD_NATIVE_WINDOW) ";
            break;
        default:
            std::cout << " (UNKNOWN EGL ERROR)\n";
            break;
        }
    }
}

void EGLX11Benchmark::setVerbosityLevel(unsigned int level)
{
    verbosity = level;
}

void EGLX11Benchmark::resetTimer(void)
{
    //// this is needed for time measuring  -->  frames per second
    gettimeofday ( &t1 , &tz );
    t2 = t1;
}

float EGLX11Benchmark::getTime(void)
{
    // Calculate elapsed time since last timer reset (comparing to t1 timestamp)
    // Result returned in seconds (float)
    gettimeofday(&t2, &tz);
    return (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
}

bool EGLX11Benchmark::userInterrupt(void)
{
    XEvent  xev;
    bool userinterrupt = false;

    // Pump all messages from X server and interpret keypresses as user interrupts
    // No other intelligence here whatsoever
    while ( XPending ( x_display ) )
    {
        XNextEvent( x_display, &xev );
        if ( xev.type == KeyPress )
            userinterrupt = true;
    }
    return userinterrupt;
}

unsigned int EGLX11Benchmark::getGLErrors(void)
{
    return GLerrors;
}

unsigned int EGLX11Benchmark::getEGLErrors(void)
{
    return EGLerrors;
}

////////////////////////////////////////////////////////////////////////////////////////////

bool EGLX11Benchmark::createEGLDisplay(int width, int height, bool fullscreen)
{
   Window root;
   XSetWindowAttributes swa;
   XSetWindowAttributes  xattr;
   Atom wm_state, a_fullscreen;
   XWMHints hints;
   XEvent xev;
   EGLConfig ecfg;
   EGLint num_config;

    w_width = width;
    w_height = height;
    w_fullscreen = fullscreen;

   /*
    * X11 native display initialization
    */

   outputMessage(5, "Connecting to X server\n");
   x_display = XOpenDisplay(NULL);
   if ( x_display == NULL )
   {
       outputMessage(1, "Error: Unable to connect to X Server\n");
       return false;
   }

   outputMessage(5, "Querying X root window\n");
   root = DefaultRootWindow(x_display);

   outputMessage(5, "Creating X11 window\n");
   swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
   win = XCreateWindow(
              x_display, root,
              0, 0, width, height, 0,
              CopyFromParent, InputOutput,
              CopyFromParent, CWEventMask,
              &swa );

   outputMessage(5, "Updating window attributes\n");
   xattr.override_redirect = false;
   XChangeWindowAttributes ( x_display, win, CWOverrideRedirect, &xattr );

   outputMessage(5, "Setting Window manager hints\n");
   hints.input = true;
   hints.flags = InputHint;
   XSetWMHints(x_display, win, &hints);

   // make the window visible on the screen
   outputMessage(5, "Making window visible\n");
   XMapWindow (x_display, win);
   XStoreName (x_display, win, "EGLX11Benchmark");

   // get identifiers for the provided atom name strings
   wm_state = XInternAtom (x_display, "_NET_WM_STATE", false);
   a_fullscreen = false;
   if (w_fullscreen == true)
       a_fullscreen = XInternAtom (x_display, "_NET_WM_STATE_FULLSCREEN", w_fullscreen);

   outputMessage(5, "Updating window event masks\n");
   memset ( &xev, 0, sizeof(xev) );
   xev.type                 = ClientMessage;
   xev.xclient.window       = win;
   xev.xclient.message_type = wm_state;
   xev.xclient.format       = 32;
   xev.xclient.data.l[0]    = 1;
   xev.xclient.data.l[1]    = a_fullscreen;
   XSendEvent (
      x_display,
      DefaultRootWindow ( x_display ),
      false,
      SubstructureNotifyMask,
      &xev );

   outputMessage(4, "X11 native display init done!\n");

   /*
    * Now that the native window is up, we shall initialize EGL
    */

   outputMessage(5, "EGL: eglGetDisplay()\n");
   egl_display  =  eglGetDisplay( (EGLNativeDisplayType) x_display );
   if ( egl_display == EGL_NO_DISPLAY ) {
       flushEGLErrors();
       outputMessage(1, "EGL: eglGetDisplay() failed!\n");
       return false;
   }
   flushEGLErrors();

   outputMessage(5, "EGL: eglInitialize()\n");
   if ( !eglInitialize( egl_display, NULL, NULL ) ) {
       flushEGLErrors();
       outputMessage(1, "EGL: eglInitialize() failed!\n");
       return false;
   }
   flushEGLErrors();

   outputMessage(5, "EGL: eglChooseConfig()\n");
   if ( !eglChooseConfig( egl_display, attr, &ecfg, 1, &num_config ) ) {
       flushEGLErrors();
       outputMessage(1, "EGL: eglChooseConfig() failed!\n");
       return false;
   }
   flushEGLErrors();

   outputMessage(5, "EGL: EGL configs available?\n");
   if ( num_config == 0 ) {
       flushEGLErrors();
       outputMessage(1, "EGL: eglGetDisplay() no configs found!\n");
       return false;
   }
   flushEGLErrors();

   outputMessage(5, "EGL: eglCreateWindowSurface()\n");
   egl_surface = eglCreateWindowSurface ( egl_display, ecfg, win, NULL );
   if ( egl_surface == EGL_NO_SURFACE ) {
       flushEGLErrors();
       outputMessage(5, "EGL: eglCreateWindowSurface() failed!\n");
       return false;
   }
   flushEGLErrors();

   outputMessage(5, "EGL: eglCreateContext()\n");
   egl_context = eglCreateContext ( egl_display, ecfg, EGL_NO_CONTEXT, ctxattr );
   if ( egl_context == EGL_NO_CONTEXT ) {
       flushEGLErrors();
       outputMessage(1, "EGL: eglCreateContext() failed!\n");
       return false;
   }
   flushEGLErrors();

   outputMessage(5, "EGL: eglMakeCurrent()\n");
   eglMakeCurrent( egl_display, egl_surface, egl_surface, egl_context );
   flushEGLErrors();
   flushGLErrors();

   outputMessage(4, "EGL initialization completed!\n");

   return true;
}

void EGLX11Benchmark::destroyEGLDisplay(void)
{
    outputMessage(4, "Starting EGL/X11 destruction\n");
    if (egl_display != 0)
    {
        if (egl_context != 0)
            eglDestroyContext (egl_display, egl_context);
        if (egl_surface != 0)
            eglDestroySurface (egl_display, egl_surface);
        eglTerminate (egl_display);
    }
    if (x_display != NULL)
    {
        if (win != 0)
            XDestroyWindow(x_display, win);
        XCloseDisplay(x_display);
    }
    egl_display = 0;
    egl_context = 0;
    egl_surface = 0;
    win = 0;
    x_display = NULL;
    outputMessage(4, "EGL/X11 Destruction done\n");
}

// Shader helpers

void EGLX11Benchmark::printShaderInfo ( GLuint shader )
{
   GLint length;

   outputMessage(5, "printShaderInfo: glGetShaderiv()\n");
   glGetShaderiv ( shader , GL_INFO_LOG_LENGTH , &length );
   flushGLErrors();

   if ( length ) {
      char buffer[length];
      GLint success;

      outputMessage(5, "printShaderInfo: glGetShaderInfoLog()\n");
      glGetShaderInfoLog ( shader , length , NULL , buffer );
      flushGLErrors();
      MESSAGE(2, "Shader info: '%s'\n", buffer);

      outputMessage(5, "printShaderInfo: glGetShaderiv()\n");
      glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
      flushGLErrors();
      if ( success != GL_TRUE )
      {
          outputMessage(1, "Error: Shader compile status failed\n");
          return;
      }
      else outputMessage(5, "printShaderInfo: OK\n");
   }
}

GLuint EGLX11Benchmark::loadShader ( const char *shader_source, GLenum type)
{
   GLuint shader;

   outputMessage(5, "loadShader: glCreateShader()\n");
   shader = glCreateShader( type );
   flushGLErrors();
   if (shader == 0)
   {
       return 0;
   }
   MESSAGE(5, "loadShader: shader handle = %d\n", shader);

   outputMessage(5, "loadShader: glShaderSource()\n");
   glShaderSource  ( shader , 1 , &shader_source , NULL );
   flushGLErrors();

   outputMessage(5, "loadShader: glCompileShader()\n");
   glCompileShader ( shader );
   flushGLErrors();

   outputMessage(5, "loadShader: Logging info...\n");
   printShaderInfo ( shader );

   outputMessage(5, "loadShader: EXIT\n");
   return shader;
}
