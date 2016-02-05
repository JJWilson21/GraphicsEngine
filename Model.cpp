/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Model Class
  Description: Implementation of Model Class
*/

/************************************************************/
// System includes
#include <vector>
#include <cstdio>
#include <cstdlib>
/************************************************************/
// Local includes

#include "Model.h"

/************************************************************/
Model::Model()
{
	natural.mat.ambientReflection.set(0.5, 0.5, 0.5);
	natural.mat.diffuseReflection.set(0, 0, 0);
	natural.mat.specularReflection.set(0.95, 0.95, 0.95);
	natural.mat.shininess = 0.9f;
}

Model::~Model()
{

}

void
Model::setShaderProgram(const ShaderProgram& shaderProgram)
{
	for (auto m : meshes)
	{
		m->setShaderProgram(shaderProgram);
	}
}

void
Model::draw(Camera& camera)
{
	for (auto m : meshes)
	{
		m->setMaterial(natural);
		m->setTexture("ballTexture.jpg");
		m->draw(camera);
	}
}

void
Model::loadModel(const std::string filepath)
{
	scene = new AiScene(filepath);
	meshes = scene->loadModel(filepath);
	for (auto m : meshes)
	{
		m = new Mesh();
		
		m->prepareVao();
		
		
	}
}

