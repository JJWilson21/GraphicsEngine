/*
  Filename   : Mesh.h
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Mesh Class
  Description: Header file for Mesh
*/

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef MESH_H      
#define MESH_H

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <memory>
#include <utility>



/************************************************************/
// Local includes

#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "Material.h"



/************************************************************/


class Mesh
{

public:

  Mesh ();

  ~Mesh ();

  void
  setShaderProgram(const ShaderProgram& shaderProgram);

  void
  addGeometry (const float geometry [], int size);
 
  void 
  addGeometry (const std::pair<VertexBuffer, IndexBuffer>);

  std::shared_ptr <VertexBuffer>
  getSharedGeometry ();

  void
  setTexture(const std::string& textureFile);

  void
  prepareVao ();
	


	void
	setName(const std::string& name);

	std::string
	getName();

  void
  draw (Camera& camera);

  // Create a copy of this Mesh and return it. Do NOT copy geometry, 
  //   the VAO, or VBO. 
  Mesh*
  clone ();

  // 18 methods from Transform
  // Use the keys specified to perform each transformation.
  // Only 10 methods methods require keys. 

  // Key: "1"
  void
  moveRight (float distance);

  void
  moveUp (float distance);

  void
  moveBack (float distance);

  void
  moveLocal (float distance, const Vector3& localDirection);

  void
  moveWorld (float distance, const Vector3& worldDirection);

  // Key: "2"
  void
  pitch (float angleDegrees);

  // Key: "3"
  void
  yaw (float angleDegrees);

  // Key: "4"
  void
  roll (float angleDegrees);

  void
  rotateLocal (float angleDegrees, const Vector3& axis);

  // Key: "5"
  void
  alignWithWorldY ();

  // Key: "6"
  void
  scaleLocal (float scale);

  void
  scaleLocal (float scaleX, float scaleY, float scaleZ);

  // Key: "7"
  void
  scaleWorld (float scale);

  void
  scaleWorld (float scaleX, float scaleY, float scaleZ);

  // Key: "8"
  void
  shearLocalXByYz (float shearY, float shearZ);

  // Key: "9"
  void
  shearLocalYByXz (float shearX, float shearZ);

  // Key: "0"
  void
  shearLocalZByXy (float shearX, float shearY);

  Math::Transform
  getTransform ();

	void
	setTransform(Math::Matrix3 m, Vector3 pos);


  void
  setCubeTexture(const std::string& filename, GLenum side);

void
setFrontGeometry(std::vector<float> geometry);
void
prepareSkyboxVao();

std::shared_ptr<VertexBuffer>
getVertexBuffer();

void
setMaterial(Material material);



private:

  //Mesh (const Mesh& mesh);

  //Mesh& operator= (const Mesh&);

  GLuint m_vaos, m_triVbo, m_numFaces, m_ebo;

	std::string name;	

  const GLuint POSITION_LOCATION = 0;

  const GLuint NORMAL_LOCATION = 1;

  const GLuint TEXTURE_LOCATION = 2;

  const float OFFSET_DELTA = 500.0f;

  ShaderProgram* g_shaderProgram;

  Math::Transform m_transform;

  Texture* m_texture;

  Material m_material;



  VertexBuffer vertexData;
  IndexBuffer index;


#define BUFFER_OFFSET(byteOffset)(reinterpret_cast<void *> (byteOffset))

};

/************************************************************/

#endif
