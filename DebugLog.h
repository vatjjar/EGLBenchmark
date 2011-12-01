
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef DebugLog_H
#define DebugLog_H

#include <stdarg.h>

class DebugLog {
public:
    DebugLog();
    ~DebugLog();

    // Methods for messages & debug output
    void outputMessage(int level, const char *message);
    void setVerbosityLevel(unsigned int level);

    void MESSAGE(int level, const char *format, ...);

private:
    int verbosity;

};

#endif // DebugLog_H
