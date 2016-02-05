/*
  Filename   : ShaderProgram.h
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Shader Class
  Description: Header file for ShaderProgram
*/   

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef SHADERPROGRAM_H         
#define SHADERPROGRAM_H

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/************************************************************/
// Local includes

#include "Vector3.h"



/************************************************************/

class ShaderProgram
{
public:

  ShaderProgram ();
  ~ShaderProgram ();

  void
  createProgramId ();

  void
  createVertexShader (const std::string& filename);

  void
  createFragmentShader (const std::string& filename);

  void
  link ();

  void
  enable ();

  void
  disable ();

  void
  compileShader (GLuint shader);

  void
  checkShaderStatus (GLuint shader, const std::string& filename);

  void
  checkProgramStatus (GLuint program);

  void
  writeToShaderLog (std::string filename, GLuint shader);

  void
  writeToProgramLog ();

  GLuint
  getAttributeLocation (const GLchar *attribute);

  GLint
  getUniformLocation (const GLchar* uniform);

  void
  setUniformMatrix4fv (GLint location, int count, GLint transpose, float* value);

  void
  setMaterial(GLint location, Vector3 data, bool vec);

  void
  setLighting(GLint location, Vector3 data);

  void
  setUniform1f(GLint location, float value);


private:

  GLuint m_programId, m_vertexShader, m_fragmentShader;

  void
  getShaderInformation (const std::string& filename, GLuint shader, bool isVertexShader);


};

/************************************************************/

#endif
