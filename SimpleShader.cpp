
/* EGLBenchmark
 *
 * Author: Jarkko Vatjus-Anttila <jvatjusanttila@gmail.com>
 *
 * For conditions of distribution and use, see copyright notice in license.txt
 */

#include "SimpleShader.h"
#include "GLWrapper.h"
#include "DebugLog.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>

SimpleShader::SimpleShader()
{
}

SimpleShader::~SimpleShader()
{
    destroyProgram();
}

/******************************************************************************
 * File I/O
 */

bool SimpleShader::fromFiles(const char *v_src, const char *f_src)
{
    programObject = createShaderProgram(v_src, f_src);
    if (programObject == 0)
    {
        DebugLog::Instance()->MESSAGE(1, "Shader program object creation failed\n");
        return false;
    }
    return true;
}

void SimpleShader::bindProgram(void)
{
    GLWrapper::Instance()->GLUSEPROGRAM(programObject);
}

void SimpleShader::linkProgram(void)
{
    GLWrapper::Instance()->GLLINKPROGRAM(programObject);
    bindProgram();
}

void SimpleShader::destroyProgram(void)
{
    if (programObject == 0)
    {
        return;
    }
    GLWrapper::Instance()->GLDELETEPROGRAM(programObject);
    GLWrapper::Instance()->GLDELETESHADER(vertexShader);
    GLWrapper::Instance()->GLDELETESHADER(fragmentShader);
    programObject = 0;
}

GLuint SimpleShader::getProgramObject(void)
{
    return programObject;
}

/******************************************************************************
 * Private methods
 */

/*
 * File I/O helper
 * ---------------
 */

/*
 * Shader program helpers
 * ----------------------
 */

bool SimpleShader::printShaderInfo ( GLuint shader )
{
   GLint length;

   GLWrapper::Instance()->GLGETSHADERIV( shader , GL_INFO_LOG_LENGTH , &length );

   if ( length )
   {
      char buffer[length];
      GLint success;

      GLWrapper::Instance()->GLGETSHADERINFOLOG( shader , length , NULL , buffer );
      DebugLog::Instance()->MESSAGE(4, "Shader info: '%s'\n", buffer);

      GLWrapper::Instance()->GLGETSHADERIV( shader, GL_COMPILE_STATUS, &success );
      if ( success != GL_TRUE )
      {
          DebugLog::Instance()->MESSAGE(4, "Error: Shader compilation failed\n");
          return false;
      }
   }
   return true;
}

GLuint SimpleShader::loadShaderProgram ( const char *shader_source, GLenum type)
{
   GLuint shader;

   shader = GLWrapper::Instance()->GLCREATESHADER( type );
   if (shader == 0)
   {
       DebugLog::Instance()->MESSAGE(2, "loadShader: shader creation failed.\n");
       return 0;
   }

   GLWrapper::Instance()->GLSHADERSOURCE( shader , 1 , &shader_source , NULL );
   GLWrapper::Instance()->GLCOMPILESHADER( shader );

   if (false == printShaderInfo ( shader ))
   {
       return 0;
   }

   return shader;
}

GLuint SimpleShader::createShaderProgram(const char *v_src, const char *f_src)
{
    GLuint shaderProgram;

    shaderProgram = GLWrapper::Instance()->GLCREATEPROGRAM();
    if (shaderProgram == 0)
    {
        DebugLog::Instance()->MESSAGE(1, "Error: Shader program creation failed\n");
        return 0;
    }

    DebugLog::Instance()->MESSAGE(4, "Initializing shaders...\n");
    if (v_src == NULL || f_src == NULL)
    {
        DebugLog::Instance()->MESSAGE(4, "Vertex nor fragment shader source must not be NULL.\n");
        return 0;
    }

    vertexShader   = loadShaderProgram ( v_src , GL_VERTEX_SHADER  );
    fragmentShader = loadShaderProgram ( f_src , GL_FRAGMENT_SHADER );
    if (vertexShader == 0 || fragmentShader == 0)
    {
        DebugLog::Instance()->MESSAGE(1, "Error: Shader program loading failed\n");
        return 0;
    }

    GLWrapper::Instance()->GLATTACHSHADER(shaderProgram, vertexShader);
    GLWrapper::Instance()->GLATTACHSHADER(shaderProgram, fragmentShader);

    return shaderProgram;
}

void SimpleShader::linkShaderProgram(GLuint shaderProgram)
{
    GLWrapper::Instance()->GLLINKPROGRAM(shaderProgram);
    GLWrapper::Instance()->GLUSEPROGRAM(shaderProgram);
}
