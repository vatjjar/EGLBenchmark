
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include <iostream>

#include "DebugLog.h"

/******************************************************************************
 * Global static instance of the class
 */

DebugLog * DebugLog::p_Instance = NULL;

DebugLog * DebugLog::Instance()
{
    if (p_Instance == NULL)
    {
        p_Instance = new DebugLog();
    }
    return p_Instance;
}

/******************************************************************************
 * Debug logging methods
 */

DebugLog::DebugLog() :
    verbosity(1)
{
}

DebugLog::~ DebugLog()
{
}

void DebugLog::MESSAGE(int level, const char *format, ...)
{
    char *buffer = new char [256];
    va_list args;
    va_start (args, format);
    vsprintf (buffer,format, args);
    outputMessage(level, buffer);
    va_end (args);
    delete buffer;
}

void DebugLog::outputMessage(int level, const char *message)
{
    if (verbosity >= level)
    {
        std::cout << level <<": " << message;
    }
}

void DebugLog::setVerbosityLevel(unsigned int level)
{
    verbosity = level;
}
