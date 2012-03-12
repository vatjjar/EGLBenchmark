
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "EGLX11Display.h"
#include "DebugLog.h"
#include "GLWrapper.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

EGLX11Display::EGLX11Display() :
    egl_context(0),
    win(0),
    x_display(NULL),
    egl_display(0),
    egl_surface(0)
{
}

EGLX11Display::~EGLX11Display()
{
    destroyEGLDisplay();
}

/******************************************************************************
 * Display and context creation helpers
 */

int EGLX11Display::createEGLDisplay(int width, int height, bool fullscreen)
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
       return -1;
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
   XStoreName (x_display, win, "EGLX11Display");

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
   GLWrapper::Instance()->flushEGLErrors();
   if ( egl_display == EGL_NO_DISPLAY ) {
       DebugLog::Instance()->MESSAGE(1, "EGL: eglGetDisplay() failed!\n");
       return -1;
   }


   DebugLog::Instance()->MESSAGE(3, "EGL: eglInitialize()\n");
   if ( !eglInitialize( egl_display, NULL, NULL ) ) {
       GLWrapper::Instance()->flushEGLErrors();
       DebugLog::Instance()->MESSAGE(1, "EGL: eglInitialize() failed!\n");
       return -1;
   }
   GLWrapper::Instance()->flushEGLErrors();

   DebugLog::Instance()->MESSAGE(3, "EGL: eglChooseConfig()\n");
   if ( !eglChooseConfig( egl_display, attr, &ecfg, 1, &num_config ) ) {
       GLWrapper::Instance()->flushEGLErrors();
       DebugLog::Instance()->MESSAGE(1, "EGL: eglChooseConfig() failed!\n");
       return -1;
   }
   GLWrapper::Instance()->flushEGLErrors();

   DebugLog::Instance()->MESSAGE(3, "EGL: EGL configs available?\n");
   if ( num_config == 0 ) {
       DebugLog::Instance()->MESSAGE(1, "EGL: eglGetDisplay() no configs found!\n");
       return -1;
   }

   DebugLog::Instance()->MESSAGE(3, "EGL: eglCreateWindowSurface()\n");
   egl_surface = eglCreateWindowSurface ( egl_display, ecfg, win, NULL );
   GLWrapper::Instance()->flushEGLErrors();
   if ( egl_surface == EGL_NO_SURFACE ) {
       DebugLog::Instance()->MESSAGE(5, "EGL: eglCreateWindowSurface() failed!\n");
       return -1;
   }


   DebugLog::Instance()->MESSAGE(3, "EGL: eglCreateContext()\n");
   egl_context = eglCreateContext ( egl_display, ecfg, EGL_NO_CONTEXT, ctxattr );
   GLWrapper::Instance()->flushEGLErrors();
   if ( egl_context == EGL_NO_CONTEXT ) {
       DebugLog::Instance()->MESSAGE(1, "EGL: eglCreateContext() failed!\n");
       return -1;
   }


   DebugLog::Instance()->MESSAGE(3, "EGL: eglMakeCurrent()\n");
   eglMakeCurrent( egl_display, egl_surface, egl_surface, egl_context );
   GLWrapper::Instance()->flushEGLErrors();
   GLWrapper::Instance()->flushGLErrors();

   DebugLog::Instance()->MESSAGE(2, "EGL initialization completed!\n");

   return 0;
}

int EGLX11Display::destroyEGLDisplay(void)
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
    return 0;
}

Display * EGLX11Display::getXDisplay(void)
{
    return x_display;
}

EGLDisplay EGLX11Display::getEGLDisplay(void)
{
    return egl_display;
}

EGLSurface EGLX11Display::getEGLSurface(void)
{
    return egl_surface;
}

unsigned int EGLX11Display::getDisplayWidth(void)
{
    DebugLog::Instance()->MESSAGE(4, "EGLX11Display: requesting display width %d\n", w_width);
    return w_width;
}

unsigned int EGLX11Display::getDisplayHeight(void)
{
    DebugLog::Instance()->MESSAGE(4, "EGLX11Display: requesting display height %d\n", w_height);
    return w_height;
}

