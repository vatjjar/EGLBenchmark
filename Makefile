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

_OBJS=                            \
    main.o                        \
    EGLX11Benchmark.o             \
    DebugLog.o                    \
    GLWrapper.o                   \
    SimpleMesh.o                  \
    SimpleTexture.o               \
    SimpleShader.o                \
    b01_ContextInit.o             \
    b02_SimpleGLShading.o         \
    b03_SimpleTriangle.o          \
    b04_ETCTextureTest.o          \
    b05_RGBTextureTest.o          \
    b06_VBOElementsRGB.o          \
    b07_PointCloud.o              \
    b08_Scenegraph.o

ODIR=objs
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))

HEADERS=                          \
    EGLX11Benchmark.h             \
    DebugLog.h                    \
    GLWrapper.h                   \
    SimpleMesh.h                  \
    SimpleTexture.h               \
    SimpleShader.h                \
    b01_ContextInit.h             \
    b02_SimpleGLShading.h         \
    b03_SimpleTriangle.h          \
    b04_ETCTextureTest.h          \
    b05_RGBTextureTest.h          \
    b06_VBOElementsRGB.h          \
    b07_PointCloud.h              \
    b08_Scenegraph.h

default: ${ODIR} all

${ODIR}:
	${MKDIR} -p ${ODIR}

${ODIR}/%.o: %.cpp ${HEADERS}
	${CPP} -c -o $@ $< $(CFLAGS) ${INCDIRS}

all: ${OBJS}
	${CPP} -o ${OUTPUT} ${OBJS} ${LIBDIRS} ${LIBS}

clean:
	rm -rf ${OBJS}
