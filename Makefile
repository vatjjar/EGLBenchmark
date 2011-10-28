# This is a very dumbass Makefile, but it suits the needs
# for a small project such as EGLBenchmark

INCDIRS=
LIBDIRS=
CFLAGS=-Wall -g
LIBS=-lEGL -lGLESv2 -lX11 -lpng
OUTPUT=EGLBenchmark

MKDIR=mkdir
CPP=g++
RM=rm

_OBJS=main.o                      \
    EGLX11Benchmark.o             \
    EGLX11Benchmark_GLWrappers.o  \
    EGLX11Benchmark_EGLWrappers.o \
    b01_ContextInit.o             \
    b02_SimpleGLShading.o         \
    b03_SimpleTriangle.o          \
    b04_ETCTextureTest.o          \
    b05_RGBTextureTest.o
ODIR=objs
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))

HEADERS=                          \
    EGLX11Benchmark.h             \
    EGLX11Benchmark_GLWrappers.h  \
    EGLX11Benchmark_EGLWrappers.h \
    b01_ContextInit.h             \
    b02_SimpleGLShading.h         \
    b03_SimpleTriangle.h          \
    b04_ETCTextureTest.h          \
    b05_RGBTextureTest.h

default: ${ODIR} all

${ODIR}:
	${MKDIR} -p ${ODIR}

${ODIR}/%.o: %.cpp ${HEADERS}
	${CPP} -c -o $@ $< $(CFLAGS) ${INCDIRS}

all: ${OBJS}
	${CPP} -o ${OUTPUT} ${OBJS} ${LIBDIRS} ${LIBS}

clean:
	rm -rf ${OBJS}
