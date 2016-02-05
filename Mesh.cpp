/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Mesh Class
  Description: Implementation of Mesh Class
*/

/************************************************************/
// System includes
#include <vector>
#include <cstdio>
#include <cstdlib>
/************************************************************/
// Local includes

#include "Mesh.h"
#include "Matrix3.h"

/************************************************************/
Mesh::Mesh()
{
  glGenVertexArrays (1, &m_vaos);
  glGenBuffers (1, &m_triVbo);
  glGenBuffers(1, &m_ebo);
  Vector3 right(1,0,0);
  Vector3 up(0,1,0);
  Vector3 back(0,0,1);
  m_transform.setRight(right);
  m_transform.setUp(up);
  m_transform.setBack(back);
  Vector3 position(0,0,0);
  m_transform.setPosition(position);


}

Mesh::~Mesh()
{
  glDeleteVertexArrays (m_vaos, &m_vaos);
  glDeleteBuffers (m_triVbo, &m_triVbo);
  delete m_texture;
}

/************************************************************/

void
Mesh::setShaderProgram(const ShaderProgram& shaderProgram)
{
  g_shaderProgram = new ShaderProgram (shaderProgram);
}


void
Mesh::addGeometry(const std::pair<VertexBuffer, IndexBuffer> pair)
{
  this->vertexData = pair.first;
  this->index = pair.second;
  m_numFaces = vertexData.size() / 18;
}

void
Mesh::setTexture(const std::string& textureFile)
{       
  m_texture = new Texture();
  m_texture->setData(textureFile);  
}




void
Mesh::prepareVao()
{
  glBindVertexArray (m_vaos);
  glBindBuffer (GL_ARRAY_BUFFER, m_triVbo);
  glBufferData (GL_ARRAY_BUFFER, vertexData.size () * sizeof (float), &vertexData.front(),
                GL_STATIC_DRAW);
        
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index.size() * sizeof(unsigned),
	  &index.front(), GL_STATIC_DRAW);

  glEnableVertexAttribArray (POSITION_LOCATION);
  glVertexAttribPointer (POSITION_LOCATION, vertexData.m_positionFloatCount, GL_FLOAT, GL_FALSE, vertexData.getVertexSizeFloats() * sizeof (float), 0);
  glEnableVertexAttribArray (NORMAL_LOCATION);
  glVertexAttribPointer (NORMAL_LOCATION, vertexData.m_normalFloatCount, GL_FLOAT, GL_FALSE, vertexData.getVertexSizeFloats() * sizeof (float),
                         BUFFER_OFFSET (3 * sizeof(float)));
  glEnableVertexAttribArray(TEXTURE_LOCATION);
  glVertexAttribPointer(TEXTURE_LOCATION, vertexData.m_textureFloatCount, GL_FLOAT, GL_FALSE, vertexData.getVertexSizeFloats() * sizeof(float),
                        BUFFER_OFFSET(6 * sizeof(float)));
        
  glBindVertexArray (0);
}

/*void
Mesh::prepareSkyboxVao()
{
	glBindVertexArray(m_vaos);
	glBindBuffer(GL_ARRAY_BUFFER, m_triVbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData->size() * sizeof(float), &vertexData->front(),
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, vertexData->m_normalFloatCount, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		BUFFER_OFFSET(3 * sizeof(float)));
	
	glEnableVertexAttribArray(TEXTURE_LOCATION);
	glVertexAttribPointer(TEXTURE_LOCATION, 0, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		BUFFER_OFFSET(6 * sizeof(float)));


	glBindVertexArray(0);
}
*/

void
Mesh::setName(const std::string& name)
{
	this->name = name;
}

std::string
Mesh::getName()
{
	return name;
}

void
Mesh::draw(Camera& camera)
{
  m_texture->bind();

  GLint modelViewLoc = g_shaderProgram->getUniformLocation("vModelView");
        
  Math::Transform viewMatrix = camera.getViewMatrix();

  Math::Transform modelMatrix = this->getTransform();

  Math::Transform viewModelMatrix = viewMatrix.combine(modelMatrix);

  float modelViewPointer[16];

  viewModelMatrix.getTransform(modelViewPointer);

  g_shaderProgram->setUniformMatrix4fv(modelViewLoc, 1, GL_FALSE,
                                       modelViewPointer);


  //g_shaderProgram->setMaterial(g_shaderProgram->getUniformLocation("material.ambientRefl"), m_material.mat.ambientReflection, true);
 // g_shaderProgram->setMaterial(g_shaderProgram->getUniformLocation("material.diffuseRefl"), m_material.mat.diffuseReflection, true);
  //g_shaderProgram->setMaterial(g_shaderProgram->getUniformLocation("material.specularRefl"), m_material.mat.specularReflection, true);
    g_shaderProgram->setMaterial(g_shaderProgram->getUniformLocation("material.diffuse"), m_material.mat.diffuseReflection, true);
  g_shaderProgram->setMaterial(g_shaderProgram->getUniformLocation("material.specular"), m_material.mat.specularReflection, true);
  g_shaderProgram->setMaterial(g_shaderProgram->getUniformLocation("material.shininess"), m_material.mat.shininess * 128.0, false);

  glBindVertexArray (m_vaos);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  m_texture->unbind();
}


// Key: "1"
void
Mesh::moveRight (float distance)
{
  m_transform.moveRight (distance);
}

void
Mesh::moveUp (float distance)
{
  m_transform.moveUp (distance);
}

void
Mesh::moveBack (float distance)
{
  m_transform.moveBack (distance);
}

void
Mesh::moveLocal (float distance, const Vector3& localDirection)
{
  m_transform.moveLocal (distance, localDirection);
}

void
Mesh::moveWorld (float distance, const Vector3& worldDirection)
{
  m_transform.moveWorld (distance, worldDirection);
}

// Key: "2"
void
Mesh::pitch (float angleDegrees)
{
  m_transform.pitch (angleDegrees);
}

// Key: "3"
void
Mesh::yaw (float angleDegrees)
{
  m_transform.yaw (angleDegrees);
}

// Key: "4"
void
Mesh::roll (float angleDegrees)
{
  m_transform.roll (angleDegrees);
}

void
Mesh::rotateLocal (float angleDegrees, const Vector3& axis)
{
  m_transform.rotateLocal (angleDegrees, axis);
}

// Key: "5"
void
Mesh::alignWithWorldY ()
{
  m_transform.alignWithWorldY ();
}

// Key: "6"
void
Mesh::scaleLocal (float scale)
{
  m_transform.scaleLocal (scale);
}

void
Mesh::scaleLocal (float scaleX, float scaleY, float scaleZ)
{
  m_transform.scaleLocal (scaleX, scaleY, scaleZ);
}

// Key: "7"
void
Mesh::scaleWorld (float scale)
{
  m_transform.scaleWorld (scale);
}

void
Mesh::scaleWorld (float scaleX, float scaleY, float scaleZ)
{
  m_transform.scaleWorld (scaleX, scaleY, scaleZ);
}

// Key: "8"
void
Mesh::shearLocalXByYz (float shearY, float shearZ)
{
  m_transform.shearLocalXByYz (shearY, shearZ);
}

// Key: "9"
void
Mesh::shearLocalYByXz (float shearX, float shearZ)
{
  m_transform.shearLocalYByXz (shearX, shearZ);
}

// Key: "0"
void
Mesh::shearLocalZByXy (float shearX, float shearY)
{
  m_transform.shearLocalZByXy (shearX, shearY);
}

Math::Transform
Mesh::getTransform ()
{
  return m_transform;
}

void
Mesh::setTransform(Math::Matrix3 m, Vector3 pos)
{
	m_transform.setOrientation(m);
	m_transform.setPosition(pos);
}

void
Mesh::setCubeTexture(const std::string& filename, GLenum side)
{
	m_texture = new Texture();
	m_texture->setSkyBoxData(filename, side);
}




void
Mesh::setMaterial(Material material)
{
	m_material = material;
}
