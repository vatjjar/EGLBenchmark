
/*
 * b06_VBOElementsRGB - benchmark #1, EGL context creation:
 * -----------------------------------------------------
 * This test case initializes native window, creates render surfaces and associated EGL context
 * to hold all of this information. No other actions are taken. This is the simplest test for EGL
 * framework.
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
bool b06_VBOElementsRGB::initBenchmark(unsigned int width, unsigned int height, bool fullscreen)
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
     * Display and context init
     */
    if (false == createEGLDisplay(width, height, fullscreen))
    {
        return false;
    }

    /*
     * Shader program init:
     */
#if 0
    shaderProgram = createShaderProgram(vertex_src, fragment_src);
    if (shaderProgram == 0)
    {
        DebugLog::Instance()->MESSAGE(1, "Error: Shader program creation failed\n");
        return false;
    }
#endif
//    GLWrapper::Instance()->GLBINDATTRIBLOCATION(shaderProgram, 0, "a_Position");
//    GLWrapper::Instance()->GLBINDATTRIBLOCATION(shaderProgram, 1, "a_Texcoord");
//    linkShaderProgram(shaderProgram);

//    texturesampler = GLWrapper::Instance()->GLGETUNIFORMLOCATION(shaderProgram, "s_texture");

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
#if 0 //defined(GL_ETC1_RGB8_OES)
        const char * filename = "resources/etctexture.pkm";
#else
        const char * filename = "resources/pngRGB.png";
#endif
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
    destroyEGLDisplay();
    return true;
}


void b06_VBOElementsRGB::Render(void)
{
    GLWrapper::Instance()->GLVIEWPORT(0, 0, w_width, w_height);
    GLWrapper::Instance()->GLCLEAR(GL_COLOR_BUFFER_BIT);
//    GLWrapper::Instance()->GLUSEPROGRAM(shaderPr);

    for (int i=0; i<TESTOBJECTS; i++)
    {
        GLWrapper::Instance()->GLACTIVETEXTURE(GL_TEXTURE0);
        st[i]->bind();
        //GLWrapper::Instance()->GLBINDTEXTURE(GL_TEXTURE_2D, textureID[i]);
        GLWrapper::Instance()->GLUNIFORM1I(texturesampler[i], 0);

        ss[i]->bindProgram();

    //    sm[i]->renderAsIndexedElements();
    //    sm[i]->renderAsIndexedElements_VBO();
    //    sm[i]->renderAsArrays();
        sm[i]->renderAsArrays_VBO();
    }

    GLWrapper::Instance()->EGLSWAPBUFFERS(egl_display, egl_surface);
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
