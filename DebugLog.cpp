
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "DebugLog.h"

/******************************************************************************
 * Debug logging methods
 */

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
