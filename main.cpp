
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include <iostream>
#include <stdlib.h>

#include "EGLX11Benchmark.h"

#include "b01_ContextInit.h"
#include "b02_SimpleGLShading.h"
#include "b03_SimpleTriangle.h"

// the default settings
static unsigned int width = 840;
static unsigned int height = 480;
static unsigned int fullscreen = false;
static int verbosity = 1;
static float duration = 5.0f;

void parseArgs(int argc, char *argv[])
{
    typedef struct _o {
        const char *name;
        unsigned int symbol;
    } OPTION;
    enum {
        OPT_VERBOSE = 1,
        OPT_WIDTH,
        OPT_HEIGHT,
        OPT_FULLSCREEN,
        OPT_DURATION,
        OPT_TESTCASE,
    };
    OPTION options[] = {
        { "-verbose",    OPT_VERBOSE },
        { "-width",      OPT_WIDTH },
        { "-height",     OPT_HEIGHT },
        { "-fullscreen", OPT_FULLSCREEN },
        { "-duration",   OPT_DURATION },
        { "-testcase",   OPT_TESTCASE },
    };

    for (int i=1; i<argc; i++)
    {
        for (unsigned int j=0; j<sizeof(options)/sizeof(OPTION); j++)
        {
            if (strncmp(argv[i], options[j].name, strlen(options[j].name)) == 0)
            {
                switch(options[j].symbol)
                {
                case OPT_VERBOSE:
                    if (i+1 == argc) break; // No params anymore
                    verbosity = atoi(argv[i+1]);
                    if (verbosity < 1) verbosity = 1;
                    if (verbosity > 5) verbosity = 5;
                    break;
                case OPT_WIDTH:
                    if (i+1 == argc) break; // No params anymore
                    width = atoi(argv[i+1]);
                    if (width < 1) width = 1;
                    break;
                case OPT_HEIGHT:
                    if (i+1 == argc) break; // No params anymore
                    height = atoi(argv[i+1]);
                    if (height < 1) height = 1;
                    break;
                case OPT_FULLSCREEN:
                    fullscreen = true;
                    break;
                case OPT_DURATION:
                    if (i+1 == argc) break; // No params anymore
                    duration = atof(argv[i+1]);
                    if (duration < 1.0f) duration = 1.0f;
                    break;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    EGLX11Benchmark *bm_table[3];

    // We have inheritance OK, so we can assume the dynamic cast to always
    // succeed!
    bm_table[0] = dynamic_cast<EGLX11Benchmark*>(new b01_ContextInit());
    bm_table[1] = dynamic_cast<EGLX11Benchmark*>(new b02_SimpleGLShading());
    bm_table[2] = dynamic_cast<EGLX11Benchmark*>(new b03_SimpleTriangle());

    parseArgs(argc, argv);

    for (unsigned int i=0; i<sizeof(bm_table)/sizeof(EGLX11Benchmark*); i++)
    {
        bm_table[i]->setVerbosityLevel(verbosity);

        if (false == bm_table[i]->initBenchmark(width, height, fullscreen))
        {
            std::cout << "initBenchmark() of '" << bm_table[i]->getName() <<"' failed\n";
            continue;
        }

        std::cout << "Benchmark name: " << bm_table[i]->getName() << "\n";
        std::cout << "Description:    " << bm_table[i]->getDescription() << "\n";
        std::cout << "Running benchmark...\n";

        if (false == bm_table[i]->runBenchmark(duration))
        {
            std::cout << "runBenchmark() FAILED!\n";
        }
        else
        {
            std::cout << "runBenchmark() SUCCESS!\n";
        }

        bm_table[i]->displayResult();
        bm_table[i]->destroyBenchmark();
    }

    std::cout << "Done!\n";
    return 0;
}
