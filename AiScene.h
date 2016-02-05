/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : AiScene
  Description: Header of AiScene class
*/   
/************************************************************/

// Macro guard
#ifndef AISCENE_H
#define AISCENE_H

/************************************************************/
//System Includes
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

/************************************************************/
// Local includes
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"


class AiScene
{

  public:

    AiScene (const std::string& fileName);

    ~AiScene ();

	std::pair<VertexBuffer, IndexBuffer>
    readVertexData (unsigned meshNum);


  private:
    // Importer dtor destroys the scene!
    Assimp::Importer m_importer;
    // Don't really need to hold this since the Importer can get it
    const aiScene* m_scene;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	std::string m_dir;

};

#endif
