
/*
 * b07_PointCloud - benchmark #7, Point cloud rendering test:
 * ----------------------------------------------------------
 * This test case creates a point cloud formation and renders it
 * via arrays of points. The idea is to test different shading of
 * the points and number of points that can be handled.
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "b07_PointCloud.h"

#include <math.h>
#include <stdlib.h>

/*************************************************************************************************
 * Pointcloud class for storing and testing the pointcloud methods
 */

typedef struct _point {
    float x, y, z;
} POINT;

class Pointcloud {
public:
    Pointcloud();
    ~Pointcloud();
    void setSize(unsigned int size);
    void generate(void);
    unsigned int getMemoryFootprint(void);
    void render(void);

protected:
    POINT *points;
    unsigned int size;

private:
    bool swirlEnvelope(float x, float y, float z);
    unsigned int pc1, pc2, pc3;
};

Pointcloud::Pointcloud() :
    points(NULL),
    size(10)
{
}

Pointcloud::~Pointcloud()
{
    if (points != NULL)
        delete points;
}

void Pointcloud::generate(void)
{
    unsigned int nPoints;
    unsigned int nFails;
    float x, y, z;

    nFails = 0;
    nPoints = 0;
    pc1 = pc2 = pc3 = 0;
    while (nPoints < size)
    {
        x = (float)rand()/(float)(RAND_MAX/2) - 1.0f;
        y = (float)rand()/(float)(RAND_MAX/2) - 1.0f;
        z = (float)rand()/(float)(RAND_MAX/2) - 1.0f;
        if (swirlEnvelope(x, y, z) == true)
        {
            nPoints++;
            points[nPoints].x = x;
            points[nPoints].y = y;
            points[nPoints].z = z;
            //std::cout << "x="<<x<<",y="<<y<<",z="<<z<<"\n";
        }
        else
        {
            nFails++;
            continue;
        }
    }
    std::cout << "fails="<<nFails<<", nPoints="<<nPoints<<"\n";
    std::cout << "pc1="<<pc1<<",pc2="<<pc2<<",pc3="<<pc3<<"\n";
}

bool Pointcloud::swirlEnvelope(float x, float y, float z)
{
    static float R = 0.3f;
    static float Pm = M_PI/2.0f;
    static float P = 2.0f;
    float rp, w;

    rp = sqrt(x*x + z*z);

    if (rp < 3*R/4) // Inner sphere
    {
        if (sqrt(x*x+y*y+z*z) <= R)
        {
            pc1++;
            return true;
        }
    }
    else // Swirling part
    {
        float envelope;
        float c;
        w = atan(z/x);
        c = cos(P*w + rp*Pm);
        envelope = ((1-rp)/2.0f)*c*c;
        if (y < envelope)
        {
            pc2++;
            return true;
        }
    }

    // If we are here, the two tests above failed
    if (fabs(y) < 0.05)
    {
        pc3++;
        return true;
    }
    return false;
}

void Pointcloud::setSize(unsigned int s)
{
    if (points != NULL)
        delete points;
    size = s;
    points = new POINT [size];
}

unsigned int Pointcloud::getMemoryFootprint(void)
{
    return size*sizeof(POINT);
}

void Pointcloud::render(void)
{
    GLWrapper::Instance()->GLVERTEXATTRIBPOINTER(0, 3, GL_FLOAT, GL_FALSE, 0, points);
    GLWrapper::Instance()->GLENABLEVERTEXATTRIBARRAY(0);
    GLWrapper::Instance()->GLDRAWARRAYS(GL_POINTS, 0, size);
}



/*************************************************************************************************
 * Constructor and destructor are dummy ones. Only descriptions are set, and other activities are
 * done when calling the virtual benchmark API
 */

b07_PointCloud::b07_PointCloud()
{
    setName("Point cloud rendering test");
    setDescription("This test generates a point cloud on the fly and renders it");
}

b07_PointCloud::~b07_PointCloud()
{
}

static Pointcloud *p;

/*
 * initBenchmark() shall initialize all required resources for this test case. If initialization fails,
 * false must be returned to indicate core benchmark not to continue execution. Parent class outputMessage()
 * method can be used to output information about the initialization
 */
bool b07_PointCloud::initBenchmark(void)
{
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

    ss = new SimpleShader();
    if (false == ss->fromFiles(vertex_src, fragment_src))
    {
        DebugLog::Instance()->MESSAGE(2, "Shader program object creation failed\n");
        return false;
    }
    GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss->getProgramObject(), 0, "vPosition");
    ss->linkProgram();

    p = new Pointcloud();
    p->setSize(100000);
    p->generate();

    GLWrapper::Instance()->GLCLEARCOLOR(0, 0, 0, 0);

    // If we have errors in GL pipe, then abort.
    if (GLWrapper::Instance()->getGLErrors() > 0) return false;

    return true;
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b07_PointCloud::destroyBenchmark(void)
{
    delete p;
    return true;
}

/*
 * renderSingleFrame()
 */
bool b07_PointCloud::renderSingleFrame(float timedelta)
{
    GLWrapper::Instance()->GLVIEWPORT(0, 0, display->getDisplayWidth(), display->getDisplayHeight());
    GLWrapper::Instance()->GLCLEAR(GL_COLOR_BUFFER_BIT);
    ss->bindProgram();
    p->render();

    // get the rendered buffer to the screen
    GLWrapper::Instance()->EGLSWAPBUFFERS(display->getEGLDisplay(), display->getEGLSurface());
    return true;
}


/*
 * getRenderStatistics()
 */
bool b07_PointCloud::getRenderStatistics(RENDER_STATISTICS *rs)
{
    return true;
}
