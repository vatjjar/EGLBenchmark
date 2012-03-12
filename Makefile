# This is a very dumbass Makefile, but it suits the needs
# for a small project such as EGLBenchmark

INCDIRS=-I./
LIBDIRS=-L./
CFLAGS=-Wall -g -fPIC
LIBS=-lEGL -lGLESv2 -lX11 -lpng
OUTPUT=libEGLBenchmark.so

MKDIR=mkdir
CPP=g++
RM=rm

_OBJS=                            \
    __API.o                       \
    EngineCore.o                  \
    EGLX11Display.o               \
    EGLX11Benchmark.o             \
    b01_ContextInit.o             \
    b02_SimpleGLShading.o         \
    b03_SimpleTriangle.o          \
    b04_ETCTextureTest.o          \
    b05_RGBTextureTest.o          \
    b06_VBOElementsRGB.o          \
    b07_PointCloud.o              \
    b08_Scenegraph.o              \
    b09_Terrain.o                 \
    SimpleShader.o                \
    SimpleMesh.o                  \
    SimpleScenegraph.o            \
    SimpleTexture.o               \
    GLWrapper.o                   \
    GLMath.o                      \
    DebugLog.o                    

ODIR=objs
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))

HEADERS=                          \
    __API.h                       \
    EngineCore.h                  \
    EGLX11Display.h               \
    EGLX11Benchmark.h             \
    b01_ContextInit.h             \
    b02_SimpleGLShading.h         \
    b03_SimpleTriangle.h          \
    b04_ETCTextureTest.h          \
    b05_RGBTextureTest.h          \
    b06_VBOElementsRGB.h          \
    b07_PointCloud.h              \
    b08_Scenegraph.h              \
    b09_Terrain.h                 \
    SimpleShader.h                \
    SimpleMesh.h                  \
    SimpleScenegraph.h            \
    SimpleTexture.h               \
    DebugLog.h                    \
    GLMath.h                      \
    GLWrapper.h

default: ${ODIR} all

${ODIR}:
	${MKDIR} -p ${ODIR}

${ODIR}/%.o: %.cpp ${HEADERS}
	${CPP} -c -o $@ $< $(CFLAGS) ${INCDIRS}

all: ${OBJS}
	${CPP} -o ${OUTPUT} -shared ${OBJS} ${LIBDIRS} ${LIBS}
	h2xml __API.h -o __API.xml -I .
	xml2py __API.xml -o __API.py -c -l ./${OUTPUT}

clean:
	rm -rf ${OBJS} ${OUTPUT}
