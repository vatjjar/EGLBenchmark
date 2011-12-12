
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include <iostream>
#include <stdlib.h>
#include <stdarg.h>

#include "EGLX11Benchmark.h"

#include "b01_ContextInit.h"
#include "b02_SimpleGLShading.h"
#include "b03_SimpleTriangle.h"
#include "b04_ETCTextureTest.h"
#include "b05_RGBTextureTest.h"
#include "b06_VBOElementsRGB.h"

#include "DebugLog.h"

// the default settings
enum {
    TEST_CONTEXTINIT = 1,
    TEST_SIMPLESHADER,
    TEST_SIMPLETRIANGLE,
    TEST_ETCTEXTURE,
    TEST_RGBTEXTURE,
    TEST_VBOELEMENTSRGB
};
static unsigned int width = 840;
static unsigned int height = 480;
static unsigned int fullscreen = false;
static unsigned int framelimit = 100;
static int verbosity = 1;
//static float duration = 5.0f;
static float fpslimit = 0.0f;
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
//        OPT_DURATION,
        OPT_FRAMELIMIT,
        OPT_TESTCASE,
        OPT_FPSLIMIT,
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
//        { "-duration",   OPT_DURATION },
        { "-framelimit", OPT_FRAMELIMIT },
        { "-testcase",   OPT_TESTCASE },
        { "-list",       OPT_LIST },
        { "-fpslimit",   OPT_FPSLIMIT },
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
//                case OPT_DURATION:
//                    if (i+1 == argc) break; // No params anymore
//                    duration = atof(argv[i+1]);
//                    if (duration < 1.0f) duration = 1.0f;
//                    break;
                case OPT_FRAMELIMIT:
                    if (i+1 == argc) break; // No params anymore
                    framelimit = atoi(argv[i+1]);
                    if (framelimit < 1) framelimit = 1;
                    break;
                case OPT_USAGE:
                case OPT_H:
                case OPT_HELP:
                    std::cout << "Usage:\n" << argv[0] << " -width <W> -height <H> -fullscreen";
                    std::cout << "  -verbose <LEVEL 1-5> -framelimit <frames> -usage|-h|-help -list -testcase <testcasename>\n";
                    return false;
                case OPT_LIST:
                    std::cout << "List of available test cases:\n";
                    std::cout << "   context:       EGL context initialization test\n";
                    std::cout << "   shader:        Simple Shader test\n";
                    std::cout << "   triangle:      Simple Triangle test\n";
                    std::cout << "   etctest:       ETC texture mapping test\n";
                    std::cout << "   rgbtest:       RGB texture mapping test\n";
                    std::cout << "   vboelementrgb: VBO rendering with elements and RGB texturing\n";
                    return false;
                case OPT_TESTCASE:
                    if (i+1 == argc) break; // No params anymore
                    if (0 == strcmp(argv[i+1], "context")) testcase = TEST_CONTEXTINIT;
                    else if (0 == strcmp(argv[i+1], "shader")) testcase = TEST_SIMPLESHADER;
                    else if (0 == strcmp(argv[i+1], "triangle")) testcase = TEST_SIMPLETRIANGLE;
                    else if (0 == strcmp(argv[i+1], "etctest")) testcase = TEST_ETCTEXTURE;
                    else if (0 == strcmp(argv[i+1], "rgbtest")) testcase = TEST_RGBTEXTURE;
                    else if (0 == strcmp(argv[i+1], "vboelementrgb")) testcase = TEST_VBOELEMENTSRGB;
                    else
                    {
                        std::cout << "Unrecognized test case: " << argv[i+1] << "\n";
                        return false;
                    }
                    break;
                case OPT_FPSLIMIT:
                    if (i+1 == argc) break; // No params anymore
                    fpslimit = atof(argv[i+1]);
                    if (fpslimit < 0.0f) fpslimit = 0.0f;
                    break;
                }
            }
        }
    }
    return true;
}

void calculateFrameVariance(float *frametimes, unsigned int samples, float *average, float *variance)
{
    float sum;

    // First an average of the frame update times
    sum = 0.0f;
    for (unsigned int i=0; i<samples; i++)
    {
        sum += frametimes[i];
    }
    *average = sum / samples;

    // and then a variance of the frame update time
    sum = 0;
    for (unsigned int i=0; i<samples; i++)
    {
        sum += ((frametimes[i] - *average)*(frametimes[i] - *average));
    }
    *variance = sum / samples;
}

int main(int argc, char *argv[])
{
    EGLX11Benchmark *bm;
    float *frametimes;

    if (false == parseArgs(argc, argv))
    {
        return 0;
    }

    // A vector to store each frame update time
    frametimes = new float[framelimit];

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
    case TEST_RGBTEXTURE:
        bm = dynamic_cast<EGLX11Benchmark*>(new b05_RGBTextureTest());
        break;
    case TEST_VBOELEMENTSRGB:
        bm = dynamic_cast<EGLX11Benchmark*>(new b06_VBOElementsRGB());
        break;
    }

    DebugLog::Instance()->setVerbosityLevel(verbosity);

    if (false == bm->initBenchmark(width, height, fullscreen))
    {
        std::cout << "initBenchmark() of '" << bm->getName() <<"' failed\n";
    }
    else
    {
        float totaltime;
        float deltatime;
        unsigned int renderedFrames;
        float average, variance;
        RENDER_STATISTICS rs;

        std::cout << "Benchmark name: " << bm->getName() << "\n";
        std::cout << "Description:    " << bm->getDescription() << "\n";
//        std::cout << "Running benchmark (duration="<<duration<<" seconds)...\n";
        std::cout << "Running benchmark (framelimit="<<framelimit<<" frames)...\n";

        // Timer and variables
        bm->resetTimer();
        totaltime = 0.0f;
        deltatime = 0.0f;
        renderedFrames = 0;

        while ( renderedFrames < framelimit )
        {
            bm->getTimeSinceLastFrame();
            if (false == bm->renderSingleFrame(deltatime))
            {
                std::cout << "Error during framerender. Abort!\n";
                break;
            }

            deltatime = bm->getTimeSinceLastFrame();
            frametimes[renderedFrames] = deltatime;
            renderedFrames++;

            // FPS limit
            if (fpslimit > 0.0f)
            {
                float frametime;
                frametime = 1.0f/fpslimit;
                if (deltatime < frametime)
                {
                    useconds_t sl = (useconds_t)((frametime-deltatime)*1000000);
                    usleep(sl);
                    deltatime = frametime;
                }
            }
            totaltime += deltatime;

            if (bm->userInterrupt() == true)
            {
                std::cout << "User interrupt!\n";
                break;
            }
        }
        calculateFrameVariance(frametimes, framelimit, &average, &variance);
        bm->getRenderStatistics(&rs);
        std::cout << "Total rendering time:             " << totaltime << "\n";
        std::cout << "Total rendered frames:            " << renderedFrames << "\n";
        std::cout << "Measurement frametime (average):  " << average << " seconds\n";
        std::cout << "Measurement frametime (variance)  " << variance << " seconds\n";
        std::cout << "Frames per second:                " << renderedFrames/totaltime << "\n";
        std::cout << "Total GL errors during the test:  " << bm->getGLErrors() << "\n";
        std::cout << "Total EGL errors during the test: " << bm->getEGLErrors() << "\n";
        std::cout << "Total rendering: "<<rs.r_vertices<<" vert, "<<rs.r_normals<<" norm, " <<
                     rs.r_texcoords<<" texc, "<<rs.r_faces<<" face, "<<rs.r_batches<<" batch\n";
        std::cout << "Total frame rendering: "<<rs.r_vertices/renderedFrames<<" vert, "<<rs.r_normals/renderedFrames<<" norm, " <<
                     rs.r_texcoords/renderedFrames<<" texc, "<<rs.r_faces/renderedFrames<<" face, "<<rs.r_batches/renderedFrames<<" batch\n";
        bm->destroyBenchmark();
    }

    std::cout << "Done!\n";
    return 0;
}
