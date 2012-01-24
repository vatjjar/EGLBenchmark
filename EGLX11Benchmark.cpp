
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "EGLX11Benchmark.h"
#include "DebugLog.h"
#include "GLWrapper.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>


EGLX11Benchmark::EGLX11Benchmark() :
    name("No name"),
    description("No description"),
    win(0),
    x_display(NULL),
    egl_display(0),
    egl_surface(0),
    egl_context(0)
{
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

void EGLX11Benchmark::setVerbosityLevel(unsigned int level)
{
    //log->setVerbosityLevel(level);
}

void EGLX11Benchmark::resetTimer(void)
{
    gettimeofday ( &t_start , &tz );
    t_now = t_lastframe = t_start;
}

float EGLX11Benchmark::getTimeSinceTimerReset(void)
{
    gettimeofday(&t_now, &tz);
    return (float)(t_now.tv_sec - t_start.tv_sec + (t_now.tv_usec - t_start.tv_usec) * 1e-6);
}

float EGLX11Benchmark::getTimeSinceLastFrame(void)
{
    float delta;
    gettimeofday(&t_now, &tz);
    delta = (float)(t_now.tv_sec - t_lastframe.tv_sec + (t_now.tv_usec - t_lastframe.tv_usec) * 1e-6);
    t_lastframe = t_now;
    return delta;
}

bool EGLX11Benchmark::userInterrupt(void)
{
    XEvent  xev;
    bool userinterrupt = false;
    char text;
    KeySym key;

    // Pump all messages from X server and interpret keypresses as user interrupts
    // No other intelligence here whatsoever
    while ( XPending ( x_display ) )
    {
        XNextEvent( x_display, &xev );
        if ( xev.type == KeyPress )
        {
            if (XLookupString(&xev.xkey,&text,1,&key,0)==1)
            {
                userinterrupt = keyHandler(text);
            }
        }
        if ( xev.type == DestroyNotify )
            userinterrupt = GL_TRUE;
    }
    return userinterrupt;
}

void EGLX11Benchmark::flushGLErrors(void)
{
    GLWrapper::Instance()->flushGLErrors();
}

void EGLX11Benchmark::flushEGLErrors(void)
{
    GLWrapper::Instance()->flushEGLErrors();
}

unsigned int EGLX11Benchmark::getGLErrors(void)
{
    return GLWrapper::Instance()->getGLErrors();
}

unsigned int EGLX11Benchmark::getEGLErrors(void)
{
    return GLWrapper::Instance()->getEGLErrors();
}

/*
 * Display and context creation helpers
 * ------------------------------------
 */

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

   DebugLog::Instance()->MESSAGE(3, "Connecting to X server\n");
   x_display = XOpenDisplay(NULL);
   if ( x_display == NULL )
   {
       DebugLog::Instance()->MESSAGE(1, "Error: Unable to connect to X Server\n");
       return false;
   }

   DebugLog::Instance()->MESSAGE(3, "Querying X root window\n");
   root = DefaultRootWindow(x_display);

   DebugLog::Instance()->MESSAGE(3, "Creating X11 window\n");
   swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
   win = XCreateWindow(
              x_display, root,
              0, 0, width, height, 0,
              CopyFromParent, InputOutput,
              CopyFromParent, CWEventMask,
              &swa );

   DebugLog::Instance()->MESSAGE(3, "Updating window attributes\n");
   xattr.override_redirect = false;
   XChangeWindowAttributes ( x_display, win, CWOverrideRedirect, &xattr );

   DebugLog::Instance()->MESSAGE(3, "Setting Window manager hints\n");
   hints.input = true;
   hints.flags = InputHint;
   XSetWMHints(x_display, win, &hints);

   // make the window visible on the screen
   DebugLog::Instance()->MESSAGE(3, "Making window visible\n");
   XMapWindow (x_display, win);
   XStoreName (x_display, win, "EGLX11Benchmark");

   // get identifiers for the provided atom name strings
   wm_state = XInternAtom (x_display, "_NET_WM_STATE", false);
   a_fullscreen = false;
   if (w_fullscreen == true)
       a_fullscreen = XInternAtom (x_display, "_NET_WM_STATE_FULLSCREEN", w_fullscreen);

   DebugLog::Instance()->MESSAGE(3, "Updating window event masks\n");
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

   DebugLog::Instance()->MESSAGE(2, "X11 native display init done!\n");

   /*
    * Now that the native window is up, we shall initialize EGL
    */

   DebugLog::Instance()->MESSAGE(3, "EGL: eglGetDisplay()\n");
   egl_display  =  eglGetDisplay( (EGLNativeDisplayType) x_display );
   if ( egl_display == EGL_NO_DISPLAY ) {
       flushEGLErrors();
       DebugLog::Instance()->MESSAGE(1, "EGL: eglGetDisplay() failed!\n");
       return false;
   }
   flushEGLErrors();

   DebugLog::Instance()->MESSAGE(3, "EGL: eglInitialize()\n");
   if ( !eglInitialize( egl_display, NULL, NULL ) ) {
       flushEGLErrors();
       DebugLog::Instance()->MESSAGE(1, "EGL: eglInitialize() failed!\n");
       return false;
   }
   flushEGLErrors();

   DebugLog::Instance()->MESSAGE(3, "EGL: eglChooseConfig()\n");
   if ( !eglChooseConfig( egl_display, attr, &ecfg, 1, &num_config ) ) {
       flushEGLErrors();
       DebugLog::Instance()->MESSAGE(1, "EGL: eglChooseConfig() failed!\n");
       return false;
   }
   flushEGLErrors();

   DebugLog::Instance()->MESSAGE(3, "EGL: EGL configs available?\n");
   if ( num_config == 0 ) {
       flushEGLErrors();
       DebugLog::Instance()->MESSAGE(1, "EGL: eglGetDisplay() no configs found!\n");
       return false;
   }
   flushEGLErrors();

   DebugLog::Instance()->MESSAGE(3, "EGL: eglCreateWindowSurface()\n");
   egl_surface = eglCreateWindowSurface ( egl_display, ecfg, win, NULL );
   if ( egl_surface == EGL_NO_SURFACE ) {
       flushEGLErrors();
       DebugLog::Instance()->MESSAGE(5, "EGL: eglCreateWindowSurface() failed!\n");
       return false;
   }
   flushEGLErrors();

   DebugLog::Instance()->MESSAGE(3, "EGL: eglCreateContext()\n");
   egl_context = eglCreateContext ( egl_display, ecfg, EGL_NO_CONTEXT, ctxattr );
   if ( egl_context == EGL_NO_CONTEXT ) {
       flushEGLErrors();
       DebugLog::Instance()->MESSAGE(1, "EGL: eglCreateContext() failed!\n");
       return false;
   }
   flushEGLErrors();

   DebugLog::Instance()->MESSAGE(3, "EGL: eglMakeCurrent()\n");
   eglMakeCurrent( egl_display, egl_surface, egl_surface, egl_context );
   flushEGLErrors();
   flushGLErrors();

   DebugLog::Instance()->MESSAGE(2, "EGL initialization completed!\n");

   return true;
}

void EGLX11Benchmark::destroyEGLDisplay(void)
{
    DebugLog::Instance()->MESSAGE(3, "Starting EGL/X11 destruction\n");
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
    DebugLog::Instance()->MESSAGE(3, "EGL/X11 Destruction done\n");
}
