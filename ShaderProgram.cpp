/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Shader Program
  Description: Implementation of Shader class
*/   

/************************************************************/
// System includes
#include <vector>
#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
/************************************************************/
// Local includes

#include "ShaderProgram.h"
#include "Material.h"

/************************************************************/
ShaderProgram::ShaderProgram ()
{
}
ShaderProgram::~ShaderProgram ()
{
  glDetachShader (m_programId, m_vertexShader);
  glDetachShader (m_programId, m_fragmentShader);
}

/************************************************************/
void
ShaderProgram::createProgramId ()
{
  m_programId = glCreateProgram ();
}



void
ShaderProgram::createVertexShader (const std::string& filename)
{
  m_vertexShader= glCreateShader (GL_VERTEX_SHADER);
  getShaderInformation (filename, m_vertexShader, true);
  glCompileShader (m_vertexShader);
  checkShaderStatus (m_vertexShader, filename);
}



void
ShaderProgram::createFragmentShader (const std::string& filename)
{
  m_fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
  getShaderInformation (filename,m_fragmentShader, false);
  glCompileShader (m_fragmentShader);
  checkShaderStatus (m_fragmentShader, filename);
}



void
ShaderProgram::link ()
{
  glAttachShader (m_programId, m_vertexShader);
  glAttachShader (m_programId, m_fragmentShader);
  glLinkProgram (m_programId);
}

void
ShaderProgram::enable ()
{
  glUseProgram (m_programId);
}



void
ShaderProgram::disable ()
{
  glUseProgram (0);
}



void
ShaderProgram::checkShaderStatus (GLuint shader, const std::string& filename)
{
  GLint compiled = 0;
  glGetShaderiv (shader, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_FALSE)
  {
    writeToShaderLog (filename, shader);
  }
}



void
ShaderProgram::checkProgramStatus (GLuint program)
{
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE)
  {
    writeToProgramLog ();
  }
}



void
ShaderProgram::writeToShaderLog (std::string filename, GLuint shader)
{
  filename.append (".log");
  const char* logFile = filename.c_str ();
  freopen (logFile, "w", stderr);
  GLint length, result;
  GLchar *log;
  glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &length);
  log = (GLchar *)malloc (length);
  glGetShaderInfoLog (shader, length, &result, log);
  fprintf (stderr, "Unable to compile. Error: %s\n", log);
  fprintf (stderr, "Shader failed to compile, exiting. \n");
  free (log);
  exit (-1);
}



void
ShaderProgram::writeToProgramLog ()
{
  freopen ("Vec2.log", "w", stderr);
  GLchar *log;
  GLint length, result;
  glGetProgramiv (m_programId, GL_INFO_LOG_LENGTH, &length);
  log = (GLchar *)malloc (length);
  glGetProgramInfoLog (m_programId, length, &result, log);
  fprintf (stderr, "Unable to compile. Error:  %s\n", log);
  fprintf (stderr , "Program failed to link. Exiting. \n");
  free (log);
  exit (-1);  
}



GLuint
ShaderProgram::getAttributeLocation (const GLchar *attribute)
{
  return glGetAttribLocation (m_programId, attribute);
}



void
ShaderProgram::getShaderInformation (const std::string& filename, GLuint shader, bool ism_vertexShader)
{
  char* shaderInformation;
  int sizeOfBuffer;
  std::ifstream shaderFile (filename, std::ios::in | std::ios::binary | std::ios::ate); 
  if (shaderFile.is_open())
  {
    sizeOfBuffer = shaderFile.tellg ();
    shaderInformation = new char[sizeOfBuffer + 1];
    shaderFile.seekg (0, std::ios::beg);
    shaderFile.read (shaderInformation, sizeOfBuffer);
    shaderFile.close ();
    shaderInformation[sizeOfBuffer] = '\0';
  }
  if (ism_vertexShader)
  {
    glShaderSource (shader, 1, &shaderInformation, 0);
  }
  else if (!ism_vertexShader)
  {
    glShaderSource (shader, 1, &shaderInformation, 0);
  }
  delete[] shaderInformation;
}


GLint
ShaderProgram::getUniformLocation(const GLchar* uniform)
{
  return glGetUniformLocation (m_programId, uniform);
}

void
ShaderProgram::setUniformMatrix4fv(GLint location, int count, GLint transpose, float* value)
{
  glUniformMatrix4fv (location, count, transpose, value);
}

void
ShaderProgram::setMaterial(GLint location, Vector3 data, bool vec)
{	
	//enable();
	if (vec)
	{
		glUniform3f(location, data.x, data.y, data.z);
	}
	else
	{
		glUniform1f(location, data.x);
	}
	//disable();
	
}

void
ShaderProgram::setLighting(GLint location, Vector3 data)
{
	//enable();
	glUniform3f(location, data.x, data.y, data.z);
	//disable();
}

void
ShaderProgram::setUniform1f(GLint location, float value)
{
	enable();
	glUniform1f(location, value);
	disable();
}



/************************************************************/




