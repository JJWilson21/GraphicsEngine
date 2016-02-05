/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Texture
  Description: Header file for texture class
*/   

/************************************************************/
// Macro Guard
#ifndef TEXTURE_H
#define TEXTURE_H

/************************************************************/
// System includes
#include <string>
#include <vector>

#include <GL/glew.h>

class Texture
{
public:

  Texture ();

  ~Texture ();

  void
  setSWrapMode (GLint sWrapMode);

  void
  setTWrapMode (GLint tWrapMode);

  void
  setMinFilter (GLint minFilter);

  void
  setMagFilter (GLint magFilter);

  void
  setData (const std::string& textureFile);

  void
  bind () const;

  void
  unbind () const;

  void
  bindCubeMap();

  void
  unbindCubeMap();

  void
  setCubeMapSWrapMode(GLint sWrapMode);

  void
  setCubeMapTWrapMode(GLint tWrapMode);

  void
  setCubeMapRWrapMode(GLint rWrapMode);

  void 
  setCubeMapMinFilter(GLint minFilter);

  void
  setCubeMapMagFilter(GLint magFilter);

  void
  setSkyBoxData(const std::string& filename, GLenum side);

private:
  GLuint m_textureId;
};

#endif 
