/*
  Filename   : Scene.cpp
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Scene Class
  Description: Implementation of Scene
*/   

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>

/************************************************************/
// Local includes

#include "Scene.h"


/************************************************************/
Scene::Scene()
{
  
}

Scene::~Scene()
{
  
}

void
Scene::add(Mesh* mesh, const std::string& name)
{
	mesh->setName(name);
	m_meshList.push_back(mesh);
}


void
Scene::remove(const std::string& name)
{
	//I was looking at doing search via the std::find but was unable to figure out how to get it effectively considering I would have
	// to compare the parameter with the function return from getName()

	for (size_t i = 0; i < m_meshList.size(); ++i)
	{
		if (m_meshList[i]->getName() == name)
		{
			delete m_meshList[i];
			m_meshList.erase(m_meshList.begin() + i);
			setActive(m_meshList.back()->getName());
			break;
		}
	}
}


void
Scene::clear()
{

	for (auto m : m_meshList)
	{
		delete m;
	}
	m_meshList.clear();
	
}
  
void
Scene::setActive(const std::string& name)
{
	for (size_t i = 0; i < m_meshList.size(); ++i)
	{
		if (m_meshList[i]->getName() == name)
		{
			std::swap(m_meshList[i], m_meshList[m_meshList.size()]);
			break;
		}
	}
}


void
Scene::draw(Camera& camera)
{
	Vector3 o(0,0,0);
	//g_shaderProgram->setLighting(g_shaderProgram->getUniformLocation("light.ambient"), m_light.l.ambient);
	g_shaderProgram->setLighting(g_shaderProgram->getUniformLocation("light.diffuse"), m_light.l.diffuse);
	g_shaderProgram->setLighting(g_shaderProgram->getUniformLocation("light.specular"), m_light.l.specular);
	g_shaderProgram->setLighting(g_shaderProgram->getUniformLocation("light.pos"), m_light.l.direction);
	//g_shaderProgram->setLighting(g_shaderProgram->getUniformLocation("light.spotDir"), o);
	//g_shaderProgram->setLighting(g_shaderProgram->getUniformLocation("light.coneCosCutoff"), o);
	//g_shaderProgram->setLighting(g_shaderProgram->getUniformLocation("light.falloff"), 0);
	//g_shaderProgram->setLighting(g_shaderProgram->getUniformLocation("light.direction"), m_light.l.direction);
	
	for (auto m : m_meshList)
	{
		m->draw(camera);
	}
}

Mesh*
Scene::getActiveMesh()
{
	return m_meshList.back();
}

size_t
Scene::size()
{
	return m_meshList.size();
}

void
Scene::removeBack()
{
	m_meshList.pop_back();
}

  
void
Scene::setShaderProgram(const ShaderProgram& shaderProgram)
{
	g_shaderProgram = new ShaderProgram(shaderProgram);
}

void
Scene::setLighting(Light& light)
{
	m_light = light;
}


