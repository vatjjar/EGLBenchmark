
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#ifndef EGLX11Benchmark_Debug_H
#define EGLX11Benchmark_Debug_H

/*
 * Simple debug output macros, which are a bit dumbass, but will do until I write
 * better ones
 */

#define MESSAGE(level, str) outputMessage(level, str);

#define MESSAGE_1P(level, str, p1) \
   { \
       char buf[128]; \
       sprintf(buf, str, p1); \
       outputMessage(level, (const char*)&buf); \
   }

#define MESSAGE_2P(level, str, p1, p2) \
   { \
       char buf[128]; \
       sprintf(buf, str, p1, p2); \
       outputMessage(level, (const char*)&buf); \
   }

#define MESSAGE_3P(level, str, p1, p2, p3) \
   { \
       char buf[128]; \
       sprintf(buf, str, p1, p2, p3); \
       outputMessage(level, (const char*)&buf); \
   }

#define MESSAGE_4P(level, str, p1, p2, p3, p4) \
   { \
       char buf[128]; \
       sprintf(buf, str, p1, p2, p3, p4); \
       outputMessage(level, (const char*)&buf); \
   }

#define MESSAGE_5P(level, str, p1, p2, p3, p4, p5) \
   { \
       char buf[128]; \
       sprintf(buf, str, p1, p2, p3, p4, p5); \
       outputMessage(level, (const char*)&buf); \
   }

#define MESSAGE_6P(level, str, p1, p2, p3, p4, p5, p6) \
   { \
       char buf[128]; \
       sprintf(buf, str, p1, p2, p3, p4, p5, p6); \
       outputMessage(level, (const char*)&buf); \
   }

#endif // EGLX11Benchmark_Debug_H
