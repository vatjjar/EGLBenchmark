
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
#include "b04_ETCTextureTest.h"

// the default settings
enum {
    TEST_CONTEXTINIT = 1,
    TEST_SIMPLESHADER,
    TEST_SIMPLETRIANGLE,
    TEST_ETCTEXTURE,
};
static unsigned int width = 840;
static unsigned int height = 480;
static unsigned int fullscreen = false;
static int verbosity = 1;
static float duration = 5.0f;
static unsigned int testcase = TEST_CONTEXTINIT;

bool parseArgs(int argc, char *argv[])
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
        OPT_LIST,
        OPT_USAGE,
        OPT_H,
        OPT_HELP,
    };
    OPTION options[] = {
        { "-verbose",    OPT_VERBOSE },
        { "-width",      OPT_WIDTH },
        { "-height",     OPT_HEIGHT },
        { "-fullscreen", OPT_FULLSCREEN },
        { "-duration",   OPT_DURATION },
        { "-testcase",   OPT_TESTCASE },
        { "-list",       OPT_LIST },
        { "-usage",      OPT_USAGE },
        { "-h",          OPT_H },
        { "-help",       OPT_HELP },
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
                case OPT_USAGE:
                case OPT_H:
                case OPT_HELP:
                    std::cout << "Usage:\n" << argv[0] << " -width <W> -height <H> -fullscreen";
                    std::cout << "  -verbose <LEVEL 1-5> -duration <seconds> -usage|-h|-help -list -testcase <testcasename>\n";
                    return false;
                case OPT_LIST:
                    std::cout << "List of available test cases:\n";
                    std::cout << "   context:  EGL context initialization test\n";
                    std::cout << "   shader:   Simple Shader test\n";
                    std::cout << "   triangle: Simple Triangle test\n";
                    std::cout << "   etctex:   ETC texture compression test\n";
                    return false;
                case OPT_TESTCASE:
                    if (i+1 == argc) break; // No params anymore
                    if (0 == strcmp(argv[i+1], "context")) testcase = TEST_CONTEXTINIT;
                    else if (0 == strcmp(argv[i+1], "shader")) testcase = TEST_SIMPLESHADER;
                    else if (0 == strcmp(argv[i+1], "triangle")) testcase = TEST_SIMPLETRIANGLE;
                    else if (0 == strcmp(argv[i+1], "etctex")) testcase = TEST_ETCTEXTURE;
                    else
                    {
                        std::cout << "Unrecognized test case: " << argv[i+1] << "\n";
                        return false;
                    }
                    break;
                }
            }
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    EGLX11Benchmark *bm;

    if (false == parseArgs(argc, argv))
    {
        return 0;
    }

    switch(testcase)
    {
    case TEST_CONTEXTINIT:
        bm = dynamic_cast<EGLX11Benchmark*>(new b01_ContextInit());
        break;
    case TEST_SIMPLESHADER:
        bm = dynamic_cast<EGLX11Benchmark*>(new b02_SimpleGLShading());
        break;
    case TEST_SIMPLETRIANGLE:
        bm = dynamic_cast<EGLX11Benchmark*>(new b03_SimpleTriangle());
        break;
    case TEST_ETCTEXTURE:
        bm = dynamic_cast<EGLX11Benchmark*>(new b04_ETCTextureTest());
        break;
    }

    bm->setVerbosityLevel(verbosity);

    if (false == bm->initBenchmark(width, height, fullscreen))
    {
        std::cout << "initBenchmark() of '" << bm->getName() <<"' failed\n";
    }
    else
    {
        std::cout << "Benchmark name: " << bm->getName() << "\n";
        std::cout << "Description:    " << bm->getDescription() << "\n";
        std::cout << "Running benchmark (duration="<<duration<<" seconds)...\n";

        if (false == bm->runBenchmark(duration))
        {
            std::cout << "runBenchmark() FAILED!\n";
        }
        else
        {
            std::cout << "runBenchmark() SUCCESS!\n";
        }

        bm->displayResult();
        bm->destroyBenchmark();
    }

    std::cout << "Done!\n";
    return 0;
}
