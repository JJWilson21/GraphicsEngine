/*
Filename   : Scene.h
Author     : Jeremiah Wilson
Course     : CSCI 375
Assignment : Scene Class
Description: Header file for Scene
*/   

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef SCENE_H   
#define SCENE_H

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>

/************************************************************/
// Local includes

#include "Vector3.h"
#include "Light.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"


/************************************************************/

class Scene
{
  
  public:

  Scene();
	
  ~Scene();

  void
  add(Mesh* mesh, const std::string& name);

  void
  remove(const std::string& name);

  void
  clear();
  
  void
  setActive(const std::string& name);

  Mesh*
  getActiveMesh();

  void
  draw(Camera& camera);

  size_t
  size();

  void
  removeBack();

  void
  setShaderProgram(const ShaderProgram& shaderProgram);

  void
  setLighting(Light& light);

  private:

  std::vector<Mesh*> m_meshList;

  Light m_light;

  ShaderProgram* g_shaderProgram;

 

};

/************************************************************/

#endif
