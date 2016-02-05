/*
  Filename   : Model.h
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Model Class
  Description: Header file for Model
*/

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef MODEL_H     
#define MODEL_H

/************************************************************/
// System includes

#include <string>


/************************************************************/
// Local includes

#include "ShaderProgram.h"
#include "AiScene.h"
#include "Texture.h"
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "Material.h"



/************************************************************/


class Model
{

public:

Model();

~Model();

 void
 setShaderProgram(const ShaderProgram& shaderProgram);

void
draw(Camera& camera);

void
loadModel(const std::string filepath);

void 
processNode(aiNode* node, const aiScene* scene);

Mesh* 
processMesh(aiMesh* mesh, const aiScene* scene);

std::vector<Texture> 
loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:

	std::vector<Mesh*> meshes;

	std::string filepath;

	Material natural;

	AiScene* scene;

};

/************************************************************/

#endif
