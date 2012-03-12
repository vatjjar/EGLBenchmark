
/*
 * b06_VBOElementsRGB - benchmark #6, VBO element RGB:
 * ---------------------------------------------------
 * This test has a number of variations implemented. It compares ETC1 and RGB texturing,
 * has external mesh loading implemented and has test code paths for Array and Element
 * rendering.
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "b06_VBOElementsRGB.h"

#include "DebugLog.h"
#include "GLWrapper.h"

/*
 * Constructor and destructor are dummy ones. Only descriptions are set, and other activities are
 * done when calling the virtual benchmark API
 */

b06_VBOElementsRGB::b06_VBOElementsRGB()
{
    setName("VBO DrawElement RGB test");
    setDescription("This test renders external mesh using VBO and DrawElements combination using RGB texturing.");
}

b06_VBOElementsRGB::~b06_VBOElementsRGB()
{
}

/*
 * initBenchmark() shall initialize all required resources for this test case. If initialization fails,
 * false must be returned to indicate core benchmark not to continue execution. DebugLog::Instance()->MESSAGE()
 * method can be used to output information about the initialization
 */
bool b06_VBOElementsRGB::initBenchmark(void)
{
    const char vertex_src[] =
       "attribute vec4 a_Position;   \n"
       "attribute vec2 a_Texcoord;   \n"
       "varying vec2 v_Texcoord;     \n"
       "void main()                  \n"
       "{                            \n"
       "   gl_Position = a_Position; \n"
       "   v_Texcoord = a_Texcoord;  \n"
       "}                            \n";
    const char fragment_src[] =
       "precision mediump float;                     \n"
       "varying vec2 v_Texcoord;                     \n"
       "uniform sampler2D s_texture;                 \n"
       "void main()                                  \n"
       "{                                            \n"
       "  gl_FragColor = texture2D(s_texture, v_Texcoord);\n"
       "}                                            \n";

    /*
     * Texture and mesh loading for the test case:
     */
    for (int i=0; i<TESTOBJECTS; i++)
    {
        // Generate meshes
        sm[i] = new SimpleMesh();
        if (false == sm[i]->fromFiles("resources/Plane"))
        {
            DebugLog::Instance()->MESSAGE(1, "Unable to load mesh from file Plane\n");
            return false;
        }

        // Generate textures
        const char * filename = "resources/pngRGB.png";
        st[i] = new SimpleTexture();
        if (false == st[i]->fromFile(filename))
        {
            DebugLog::Instance()->MESSAGE(1, "Error: Loading of texturefile '%s' failed.\n", filename);
            return false;
        }

        // Generate shader progams:
        ss[i] = new SimpleShader();
        if (false == ss[i]->fromFiles(vertex_src, fragment_src))
        {
            DebugLog::Instance()->MESSAGE(2, "Shader program object creation failed\n");
            return false;
        }
        GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss[i]->getProgramObject(), 0, "a_Position");
        GLWrapper::Instance()->GLBINDATTRIBLOCATION(ss[i]->getProgramObject(), 1, "a_Texcoord");
        ss[i]->linkProgram();
        texturesampler[i] = GLWrapper::Instance()->GLGETUNIFORMLOCATION(ss[i]->getProgramObject(), "s_texture");
    }

    GLWrapper::Instance()->GLCLEARCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

    // If we have errors in GL pipe, then abort.
    if (GLWrapper::Instance()->getGLErrors() > 0) return false;

    return true;
}

/*
 * destroyBenchmark() shall free all resources allocated by the initBenchmark() method. The core shall
 * call this method once the benchmark case has been run.
 */
bool b06_VBOElementsRGB::destroyBenchmark(void)
{
    for (int i=0; i<TESTOBJECTS; i++)
    {
        delete sm[i];
        delete st[i];
        delete ss[i];
    }
    return true;
}


void b06_VBOElementsRGB::Render(void)
{
    GLWrapper::Instance()->GLVIEWPORT(0, 0, display->getDisplayWidth(), display->getDisplayHeight());
    GLWrapper::Instance()->GLCLEAR(GL_COLOR_BUFFER_BIT);

    for (int i=0; i<TESTOBJECTS; i++)
    {
        GLWrapper::Instance()->GLACTIVETEXTURE(GL_TEXTURE0);
        st[i]->bind();
        GLWrapper::Instance()->GLUNIFORM1I(texturesampler[i], 0);

        ss[i]->bindProgram();

    //    sm[i]->renderAsIndexedElements();
    //    sm[i]->renderAsIndexedElements_VBO();
    //    sm[i]->renderAsArrays();
        sm[i]->renderAsArrays_VBO();
    }

    // get the rendered buffer to the screen
    GLWrapper::Instance()->EGLSWAPBUFFERS(display->getEGLDisplay(), display->getEGLSurface());
}


/*
 * renderSingleFrame()
 */
bool b06_VBOElementsRGB::renderSingleFrame(float deltatime)
{
    Render();
    return true;
}

/*
 * getRenderStatistics()
 */
bool b06_VBOElementsRGB::getRenderStatistics(RENDER_STATISTICS *rs)
{
    memset(rs, 0, sizeof(RENDER_STATISTICS));
    for (int i=0; i<TESTOBJECTS; i++)
    {
        rs->r_vertices  += sm[i]->getRenderedVertices();
        rs->r_normals   += sm[i]->getRenderedNormals();
        rs->r_texcoords += sm[i]->getRenderedTexcoords();
        rs->r_faces     += sm[i]->getRenderedFaces();
        rs->r_batches   += sm[i]->getRenderedBatches();
    }
    return true;
}
