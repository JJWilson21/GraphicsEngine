/*
Filename   : Skybox.h
Author     : Jeremiah Wilson
Course     : CSCI 375
Assignment : Skybox Class
Description: Header file for Skybox.cpp
*/

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef SKYBOX_H     
#define SKYBOX_H

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <memory>

/************************************************************/
// Local includes
#include "Texture.h"
#include "VertexBuffer.h"
#include "ShaderProgram.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "AiScene.h"



class Skybox
{
	public:

		Skybox();
		
		~Skybox();
	
		void
		addFrontGeometry(std::vector<float> geometry);

		void
		setFront(const std::string& filename);

		void
			setBack(const std::string& filename);

		void
			setRight(const std::string& filename);

		void
			setLeft(const std::string& filename);

		void
			setTop(const std::string& filename);

		void
			setBottom(const std::string& filename);

		void
		setSWrapMode(GLint sWrapMode);

		void
		setTWrapMode(GLint tWrapMode);
		
		void
		setRWrapMode(GLint rWrapMode);

		void
		setMinFilter(GLint minFilter);

		void
		setMagFilter(GLint magFilter);

		void
		bind();
		
		void
		unbind();

		void
		setShaderProgram(ShaderProgram& shader);

		void
		draw(Camera& camera);

void
moveRight(float distance);

void
moveBack(float distance);

	private:

		Texture* m_skyboxTexture;

		GLuint m_vaos, m_triVbo, numFaces;

		#define BUFFER_OFFSET(byteOffset)(reinterpret_cast<void *> (byteOffset))

		const GLuint POSITION_LOCATION = 0;

		const GLuint NORMAL_LOCATION = 1;
	
		ShaderProgram* g_shader;

		Mesh* front;
	
		Mesh* back;

		Mesh* right;

		Mesh* left;

		Mesh* top;

		Mesh* bottom;

		Material natural;
	
		std::vector<Mesh*> meshes;



};




#endif
