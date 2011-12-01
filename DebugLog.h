
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef DebugLog_H
#define DebugLog_H

#include <stdarg.h>
#include <stdio.h>

class DebugLog {
public:
    static DebugLog * Instance();

    ~DebugLog();

    // Methods for messages & debug output
    void outputMessage(int level, const char *message);
    void setVerbosityLevel(unsigned int level);

    void MESSAGE(int level, const char *format, ...);

private:
    DebugLog();                                     // Private constructor
    DebugLog(DebugLog const &) {};
    static DebugLog *p_Instance;                    // Single instance placeholder

    int verbosity;
};

#endif // DebugLog_H
