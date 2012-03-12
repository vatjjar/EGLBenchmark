
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
    display(NULL)
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

int EGLX11Benchmark::setEGLDisplay(EGLX11Display *d)
{
    display = d;
    return 0;
}

bool EGLX11Benchmark::userInterrupt(void)
{
    XEvent  xev;
    bool userinterrupt = false;
    char text;
    KeySym key;

    // Pump all messages from X server and interpret keypresses as user interrupts
    // No other intelligence here whatsoever
    while ( XPending ( display->getXDisplay() ) )
    {
        XNextEvent( display->getXDisplay(), &xev );
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
