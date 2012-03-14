#!/usr/bin/python
#
# EGLBenchmark
#
# Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
#
# For conditions of distribution and use, see copyright notice in license.txt
#

import sys, os

def run_benchmark(xsize, ysize, fullscreen, verbose, framelimit, testcase, fpslimit):
    #print ("xsize=%d, ysize=%d, fullscreen=%d, verbose=%d, framelimit=%d, testcase=%d:%s, fpslimit=%d" %
    #    (xsize, ysize, fullscreen, verbose, framelimit, testcase, tName[testcase-1], fpslimit))
    import __API, time

    __API.initCore()
    __API.setVerbosityLevel(verbose)

    if 0 != __API.createEGLDisplay(xsize, ysize, fullscreen):
        print "Error in creating EGL display!"
        __API.destroyCore()
        return

    if 0 != __API.initBenchmark(testcase):
        print "Error in initializing benchmark ID=%d" % testcase
        __API.destroyCore()
        return

    starttime = time.time()
    if 0 != __API.runBenchmark(fpslimit, framelimit):
        print "Error during benchmark execution"
    else:
        endtime = time.time()
        print "Total time elapsed    %2.3f" % (endtime - starttime)
        print "Total frames rendered %3d" % framelimit
        print "Frames / second       %2.4f" % (framelimit/(endtime-starttime))
        print "Average frame time    %2.3f ms" % ((endtime-starttime)/framelimit)

    __API.destroyBenchmark()
    __API.destroyCore()

if __name__ == "__main__":
    import getopt

    tName = [ "context", "shader", "triangle", "etctest", "rgbtest", "vboelementrgb", "pointcloud", "scenegraph", "terrain" ]
    c = 1
    tDict = {}
    for n in tName:
        tDict[n] = c
        c += 1

    def usage():
        print "launcher.py -x <xsize> -y <ysize> -fullscreen -verbose <N> -framelimit <N> -usage|-h|-help -list -testcase <name> -fpslimit <fps>"

    try:
        opts, args = getopt.getopt(sys.argv[1:], "x:y:fv:F:h:LTS:",
            ["xsize=", "ysize=", "fullscreen", "framelimit=", "usage", "help", "list", "testcase=", "fpslimit=", "verbose="])
    except getopt.GetoptError, err:
        print "Options error"
        usage()
        sys.exit(0)

    xsize = 800
    ysize = 480
    fullscreen = 0
    verbose = 1
    framelimit = 100
    testcase = 1
    fpslimit = 0
    for o, a in opts:
        if o in ("-x", "--xsize"):
            xsize = int(a)
            if xsize < 1: xsize = 1;
        if o in ("-y", "--xyize"):
            ysize = int(a)
            if ysize < 1: ysize = 1;
        if o in ("--fullscreen"):
            fullscreen = 1
        if o in ("-h", "--help", "--usage"):
            usage()
            sys.exit(0)
        if o in ("-v", "--verbose"):
            verbose = int(a)
            if verbose < 1: verbose = 1
            if verbose > 5: verbose = 5
        if o in ("-F", "--framelimit"):
            framelimit = int(a)
            if framelimit < 0: framelimit = 0
        if o in ("-T", "--testcase"):
            try: testcase = tDict[str(a)]
            except KeyError:
                print "No such test case %s" % str(a)
                sys.exit(0)
        if o in ("-S", "--fpslimit"):
            fpslimit = int(a)
            if fpslimit < 0: fpslimit = 0
        if o in ("--list"):
            print "Available test cases: "
            for i in tName:
                print "  %s" % i
            sys.exit(0)

    run_benchmark(xsize, ysize, fullscreen, verbose, framelimit, testcase, fpslimit)
    print "Done!"
