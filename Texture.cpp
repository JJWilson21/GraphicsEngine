/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Texture
  Description: Implementation of Texture class
*/   

/************************************************************/
// System includes
#include <cstdlib>
#include <cassert>

#include <FreeImagePlus.h>
/************************************************************/
// Local includes
#include "Texture.h"

// Use this for stb_image
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
// END stb_image

Texture::Texture ()
{
  glGenTextures (1, &m_textureId);
  // Wrap modes default to GL_REPEAT
  // Min filter defaults to GL_NEAREST_MIPMAP_LINEAR
  // Mag filter defaults to GL_LINEAR
}

Texture::~Texture ()
{
  glDeleteTextures (1, &m_textureId);
}

// Options are GL_REPEAT (default), GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE,
//   and GL_CLAMP_TO_BORDER
void
Texture::setSWrapMode (GLint sWrapMode)
{
  this->bind ();
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrapMode);
  this->unbind ();
}

void
Texture::setTWrapMode (GLint tWrapMode)
{
  this->bind ();
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapMode);
  this->unbind ();
}

// Options are GL_NEAREST, GL_LINEAR,
//   GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
//   GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
void
Texture::setMinFilter (GLint minFilter)
{
  this->bind ();
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  this->unbind ();
}

// Options are GL_NEAREST, GL_LINEAR
void
Texture::setMagFilter (GLint magFilter)
{
  this->bind ();
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  this->unbind ();
}

void
Texture::setData (const std::string& textureFile)
{
  this->bind ();
  // If using FreeImage the next two lines will load the image.
  //You can access the pixels with the "accessPixels" method of fipImage.
  //See FreeImageDemo.cc under the Code directory. 
  fipImage image;
  bool loadedOk = image.load (textureFile.c_str ());
  if (!loadedOk)
  {
    fprintf(stderr, "Failed to load image file %s\n", textureFile.c_str());
    exit(-1);
  }
  GLsizei width, height;
  //int numChannels;
  width = image.getWidth();
  height = image.getHeight();
  FREE_IMAGE_TYPE typeImage = image.getImageType();
  assert(typeImage == FIT_BITMAP);
  // unsigned char* texels = stbi_load (textureFile.c_str(), &width, &height,
  //                         &numChannels, 0);
  // if (texels == nullptr)
  // {
  //   fprintf (stderr, "Failed to load image file %s\n", textureFile.c_str ());
  //   exit (-1);
  //  }
  // Level should be 0, unless manually specifying mipmaps
  const GLint level = 0;
  GLint internalFormat = GL_RGB;
  // Border is a legacy parameter and should be set to 0
  const GLint border = 0;
  GLenum format = GL_BGR;
  GLenum type = GL_UNSIGNED_BYTE;
  //unsigned char* image =
  // FreeImage_GetBits(textureFile &width, &height, 0, SOIL_LOAD_RGB);
  //unsigned char* pixeles = (unsigned char*)FreeImage_GetBits(image);
  unsigned char* pixeles = image.accessPixels();
  // Upload texture data to OpenGL.
  glTexImage2D (GL_TEXTURE_2D, level, internalFormat, width, height, border,
                format, type, pixeles);
  // Don't generate mipmaps yet. Still testing.
  glGenerateMipmap (GL_TEXTURE_2D);
  unbind ();
  // No longer need texture data since OpenGL has it.
  //stbi_image_free (texels);
  //FreeImage_Unload(image);
}

void
Texture::bind () const
{
  glBindTexture (GL_TEXTURE_2D, m_textureId);
}

void
Texture::unbind () const
{
  glBindTexture (GL_TEXTURE_2D, 0);
}

void
Texture::bindCubeMap()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
}

void
Texture::unbindCubeMap()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void
Texture::setCubeMapSWrapMode(GLint sWrapMode)
{
	this->bindCubeMap();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, sWrapMode);
	this->unbindCubeMap();
}

void
Texture::setCubeMapTWrapMode(GLint tWrapMode)
{
	this->bindCubeMap();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tWrapMode);
	this->unbindCubeMap();
}

void
Texture::setCubeMapRWrapMode(GLint rWrapMode)
{
	this->bindCubeMap();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, rWrapMode);
	this->unbindCubeMap();
}

void
Texture::setCubeMapMinFilter(GLint minFilter)
{
	this->bindCubeMap();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
	this->unbindCubeMap();
}

void
Texture::setCubeMapMagFilter(GLint magFilter)
{
	this->bindCubeMap();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
	this->unbindCubeMap();
}

void
Texture::setSkyBoxData(const std::string& filename, GLenum side)
{
	bindCubeMap();
	fipImage image;
	const GLint level = 0;
	GLint internalFormat = GL_RGB;
	// Border is a legacy parameter and should be set to 0
	const GLint border = 0;
	GLenum format = GL_BGR;
	GLenum type = GL_UNSIGNED_BYTE;
	//bool loadedOk = image.load(textures[x].c_str());
	bool loadedOk = image.load(filename.c_str());
	if (!loadedOk)
	{
		fprintf(stderr, "Failed to load image file %s\n", filename.c_str());
		exit(-1);
	}
		
	GLsizei width, height;
	width = image.getWidth();
	height = image.getHeight();
	FREE_IMAGE_TYPE typeImage = image.getImageType();
	assert(typeImage == FIT_BITMAP);
	unsigned char* pixeles = image.accessPixels();
		// Upload texture data to OpenGL.
	glTexImage2D(side, level, internalFormat, width, height, border,

	format, type, pixeles);
	glGenerateMipmap(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP);
	setMagFilter(GL_LINEAR);
	setMinFilter(GL_LINEAR);
	setCubeMapSWrapMode(GL_CLAMP_TO_EDGE);
	setCubeMapTWrapMode(GL_CLAMP_TO_EDGE);
	setCubeMapRWrapMode(GL_CLAMP_TO_EDGE);
	unbindCubeMap();
}
