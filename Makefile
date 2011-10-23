# This is a very dumbass Makefile, but it suits the needs
# for a small project such as EGLBenchmark

INCDIRS=
LIBDIRS=
CFLAGS=-Wall -g
LIBS=-lEGL -lGLESv2 -lX11
OUTPUT=EGLBenchmark

SRC=main.cpp                \
    EGLX11Benchmark.cpp     \
    b01_ContextInit.cpp     \
    b02_SimpleGLShading.cpp \
    b03_SimpleTriangle.cpp  \
    b04_ETCTextureTest.cpp

all:
	g++ ${SRC} -o ${OUTPUT} ${CFLAGS} ${INCDIRS} ${LIBDIRS} ${LIBS}

