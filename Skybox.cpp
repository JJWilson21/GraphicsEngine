
/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Skybox
  Description: Implementation of Skybox class
*/   

/************************************************************/

// Local includes
#include "Skybox.h"

Skybox::Skybox()
{
	natural.mat.ambientReflection.set(0.5, 0.5, 0.5);
	natural.mat.diffuseReflection.set(0, 0, 0);
	natural.mat.specularReflection.set(0.95, 0.95, 0.95);
	natural.mat.shininess = 0.9f;
}

Skybox::~Skybox()
{

}


void
Skybox::setFront(const std::string& filename)
{
	AiScene planeScene("plane.obj");
	front = new Mesh();
	front->addGeometry(planeScene.readVertexData(0));
	front->prepareVao();
	front->setTexture(filename);
	front->setMaterial(natural);
	front->moveBack(-39.5f);
	front->scaleLocal(3);
	meshes.push_back(front);

}

void
Skybox::setBack(const std::string& filename)
{
	AiScene planeScene("plane.obj");
	back = new Mesh();
	back->addGeometry(planeScene.readVertexData(0));
	//back = front->clone();
	back->prepareVao();
	back->setTexture(filename);
	back->setMaterial(natural);
	back->yaw(180.0f);
	back->moveBack(-81.65f);
	back->moveRight(-3);
	back->scaleLocal(3);
	meshes.push_back(back);
}

void
Skybox::setRight(const std::string& filename)
{
	AiScene planeScene("plane.obj");
	right = new Mesh();
	right->addGeometry(planeScene.readVertexData(0));
	//right = front->clone();
	right->prepareVao();
	right->setTexture(filename);
	right->setMaterial(natural);
	right->yaw(-90.0f);
	right->moveBack(-62.5f);
	right->moveRight(19.8f);
	right->scaleLocal(3);
	meshes.push_back(right);
}

void
Skybox::setLeft(const std::string& filename)
{
	AiScene planeScene("plane.obj");
	left = new Mesh();
	left->addGeometry(planeScene.readVertexData(0));
//	left = front->clone();
	left->prepareVao();
	left->setTexture(filename);
	left->setMaterial(natural);
	left->yaw(90.5f);
	left->moveBack(-59.5f);
	left->moveRight(-21.8f);
	left->scaleLocal(3);
	meshes.push_back(left);
}

void
Skybox::setTop(const std::string& filename)
{
	AiScene planeScene("plane.obj");
	top = new Mesh();
	top->addGeometry(planeScene.readVertexData(0));
//	top = front->clone();
	top->setTexture(filename);
	top->setMaterial(natural);
	top->prepareVao();
	top->yaw(90.0f);
	//top->moveBack(-19.75f);
	//top->moveRight(-20.8f);
	top->moveUp(20.6f);
	top->pitch(90);
	top->moveRight(-20.5f);
	top->moveUp(1);
	top->roll(-90.0f);
	top->moveBack(-40.0f);
	top->scaleLocal(3);
	top->moveRight(-0.5f);
	meshes.push_back(top);
} 

void
Skybox::setBottom(const std::string& filename)
{
	AiScene planeScene("plane.obj");
	bottom = new Mesh();
	bottom->addGeometry(planeScene.readVertexData(0));
	bottom->setTexture(filename);
	bottom->setMaterial(natural);
	bottom->yaw(-90);
	//top->moveBack(-19.75f);
	//top->moveRight(-20.8f);
	bottom->moveUp(-20);
	bottom->pitch(-90);
	bottom->moveRight(20);
	bottom->moveUp(1);
	bottom->roll(-90);
	meshes.push_back(bottom);
}

void
Skybox::setSWrapMode(GLint sWrapMode)
{
	m_skyboxTexture->setCubeMapSWrapMode(sWrapMode);
}

void
Skybox::setTWrapMode(GLint tWrapMode)
{
	m_skyboxTexture->setTWrapMode(tWrapMode);
}

void
Skybox::setRWrapMode(GLint rWrapMode)
{
	m_skyboxTexture->setCubeMapRWrapMode(rWrapMode);
}

void
Skybox::setMinFilter(GLint minFilter)
{
	m_skyboxTexture->setCubeMapMinFilter(minFilter);
}

void
Skybox::setMagFilter(GLint magFilter)
{
	m_skyboxTexture->setCubeMapMagFilter(magFilter);
}

void
Skybox::bind()
{
	m_skyboxTexture->bindCubeMap();
}

void
Skybox::unbind()
{
	m_skyboxTexture->unbindCubeMap();
}

void
Skybox::setShaderProgram(ShaderProgram& shader)
{
	for (auto m : meshes)
	{
		m->setShaderProgram(shader);
	}
}

void
Skybox::draw(Camera& camera)
{

	for (auto m : meshes)
	{
		m->draw(camera);
	}
}

void
Skybox::moveRight(float distance)
{
	
}

void
Skybox::moveBack(float distance)
{

}
