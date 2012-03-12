
/*
 * b09_Terrain - benchmark #9, terrain render test:
 * ----------------------------------------------------------
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "b09_Terrain.h"

static QUADTREENODE quadtree[] = {
    {  0.0f, 0.0f,  0.0f, 10.0, 10.0f, 10.0f, {1, 2, 3, 4} },
    {  2.5f, 0.0f,  2.5f,  5.0,  5.0f,  5.0f, {0, 0, 0, 0} },
    {  2.5f, 0.0f, -2.5f,  5.0,  5.0f,  5.0f, {0, 0, 0, 0} },
    { -2.5f, 0.0f, -2.5f,  5.0,  5.0f,  5.0f, {0, 0, 0, 0} },
    { -2.5f, 0.0f,  2.5f,  5.0,  5.0f,  5.0f, {0, 0, 0, 0} },
};

/*************************************************************************************************
 * Constructor and destructor are dummy ones. Only descriptions are set, and other activities are
 * done when calling the virtual benchmark API
 */

b09_Terrain::b09_Terrain()
{
    setName("Terrain rendering test");
    setDescription("This test renderd a geomipmapped terrain");
}

b09_Terrain::~b09_Terrain()
{
}

/*
 * initBenchmark() shall initialize all required resources for this test case. If initialization fails,
 * false must be returned to indicate core benchmark not to continue execution. Parent class outputMessage()
 * method can be used to output information about the initialization
 */
bool b09_Terrain::initBenchmark(void)
{
    const char *terrainfilename = "./resources/terrain.ntf";
    const char vertex_src[] =
       "attribute vec4 vPosition;    \n"
       "void main()                  \n"
       "{                            \n"
       "   gl_Position = vPosition;  \n"
       "}                            \n";

    const char fragment_src[] =
       "precision mediump float;\n"\
       "void main()                                  \n"
       "{                                            \n"
       "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
       "}                                            \n";

    // Init terrain shader
    ss = new SimpleShader();
    if (false == ss->fromFiles(vertex_src, fragment_src))
    {
        DebugLog::Instance()->MESSAGE(2, "Shader program object creation failed\n");
        return false;
    }
    GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 0, "vPosition");
    ss->linkProgram();

    sm = new SimpleMesh();
    buildTerrainPatch();

    // Load geometry texture
    st = new SimpleTexture();
    hMap = st->readBinaryFile(terrainfilename, hMapLen);
    if (hMap == NULL)
    {
        DebugLog::Instance()->MESSAGE(1, "Loading of %s failed!\n", terrainfilename);
        return false;
    }
    if (1) // some debug
    {
        unsigned int *header;
        float *data;
        header = (unsigned int*)hMap;
        data = (float*)&hMap[8];
        DebugLog::Instance()->MESSAGE(1, "len %d, size %d %d, first elements %f %f %f\n",
                                      hMapLen, header[0], header[1], data[0], data[1], data[2]);
    }

    GLWrapper::Instance()->GLCLEARCOLOR(0, 0, 0, 0);
    GLWrapper::Instance()->GLVIEWPORT(0, 0, display->getDisplayWidth(), display->getDisplayHeight());

    // If we have errors in GL pipe, then abort.
    if (GLWrapper::Instance()->getGLErrors() > 0) return false;

    return true;
}

typedef struct _vertex {
    GLfloat x, y, z;
} VERTEX;
typedef struct _face {
    GLshort v1, v2, v3;
} FACE;

void b09_Terrain::buildTerrainPatch(void)
{
    const int cPatchSize = 16;
    VERTEX *vertices = new VERTEX [cPatchSize*cPatchSize];
    FACE *faces = new FACE [2*(cPatchSize-1)*(cPatchSize-1)];

    for (int i=0; i<cPatchSize; i++)
    {
        for (int j=0; j<cPatchSize; j++)
        {
            vertices[j+i*cPatchSize].x = (float)j/(float)(cPatchSize-1);
            vertices[j+1*cPatchSize].y = 0.0f;
            vertices[j+i*cPatchSize].z = (float)i/(float)(cPatchSize-1);
            DebugLog::Instance()->MESSAGE(1, "vert %f %f %f\n", vertices[j+i*cPatchSize].x, vertices[j+i*cPatchSize].y, vertices[j+i*cPatchSize].z);
        }
    }

    for (int i=0; i<cPatchSize-1; i++)
    {
        for (int j=0; j<cPatchSize-1; j++)
        {
            faces[2*i*(cPatchSize-1)+j*2+0].v1 = j + i*cPatchSize + 0;
            faces[2*i*(cPatchSize-1)+j*2+0].v2 = j + i*cPatchSize + cPatchSize;
            faces[2*i*(cPatchSize-1)+j*2+0].v3 = j + i*cPatchSize + cPatchSize+1;
            DebugLog::Instance()->MESSAGE(1, "face %d %d %d\n", faces[2*i*(cPatchSize-1)+j*2+0].v1, faces[2*i*(cPatchSize-1)+j*2+0].v2, faces[2*i*(cPatchSize-1)+j*2+0].v3);
            faces[2*i*(cPatchSize-1)+j*2+1].v1 = j + i*cPatchSize + 0;
            faces[2*i*(cPatchSize-1)+j*2+1].v2 = j + i*cPatchSize + cPatchSize+1;
            faces[2*i*(cPatchSize-1)+j*2+1].v3 = j + i*cPatchSize + 1;
            DebugLog::Instance()->MESSAGE(1, "face %d %d %d\n", faces[2*i*(cPatchSize-1)+j*2+1].v1, faces[2*i*(cPatchSize-1)+j*2+1].v2, faces[2*i*(cPatchSize-1)+j*2+1].v3);
        }
    }
    sm->attachVertexData(cPatchSize*cPatchSize, (GLfloat*)vertices);
    sm->attachFaceData((cPatchSize-1)*(cPatchSize-1)*2, (GLushort*)faces);
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b09_Terrain::destroyBenchmark(void)
{
    return true;
}

/*
 * renderSingleFrame()
 */
bool b09_Terrain::renderSingleFrame(float timedelta)
{
    GLWrapper::Instance()->GLVIEWPORT(0, 0, display->getDisplayWidth(), display->getDisplayHeight());
    GLWrapper::Instance()->GLCLEAR(GL_COLOR_BUFFER_BIT);
    ss->bindProgram();
    sm->renderAsIndexedElements();

    // get the rendered buffer to the screen
    GLWrapper::Instance()->EGLSWAPBUFFERS(display->getEGLDisplay(), display->getEGLSurface());
    return true;
}


/*
 * getRenderStatistics()
 */
bool b09_Terrain::getRenderStatistics(RENDER_STATISTICS *rs)
{
    return true;
}
