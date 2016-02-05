/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : AiScene
  Description: Implementation of AiScene class
*/   
/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <assimp/postprocess.h>
/************************************************************/
// Local includes

#include "AiScene.h"
#include "Transform.h"

AiScene::AiScene (const std::string& fileName)
{
  m_scene = m_importer.ReadFile (fileName,
                                 aiProcess_Triangulate
                                 | aiProcess_GenNormals
                                 | aiProcess_GenUVCoords
                                 | aiProcess_JoinIdenticalVertices);
 
  
  if (!m_scene)
  {
    auto error = m_importer.GetErrorString ();
    fprintf (stderr, "Failed to load model %s. Error: %s\n",
             fileName.c_str (), error);
    exit (-1);
  }
}

AiScene::~AiScene ()
{
  // Nothing to do as Importer dtor cleans up
}

std::pair<VertexBuffer, IndexBuffer>
AiScene::readVertexData (unsigned meshNum)
{
  // Load data from mesh number "meshNum"
  vertexBuffer = new VertexBuffer();
  indexBuffer = new IndexBuffer();
  vertexBuffer->setVertexSizeFloats(8);
  const aiMesh* mesh = m_scene->mMeshes[meshNum];
  for (unsigned vertexNum = 0; vertexNum < mesh->mNumVertices; ++vertexNum)
  {
    aiVector3D position = mesh->mVertices[vertexNum];
    vertexBuffer->addData (position, 3);
    aiVector3D normal = mesh->mNormals[vertexNum];
    vertexBuffer->addData (normal, 3);
    aiVector3D texCoords = mesh->mTextureCoords[0][vertexNum];
    vertexBuffer->addData (texCoords, 2);
  }
  const unsigned INDICES_PER_FACE = 3;
  for (unsigned faceNum = 0; faceNum < mesh->mNumFaces; ++faceNum)
  {
    const aiFace& face = mesh->mFaces[faceNum];
    for (unsigned x = 0; x < INDICES_PER_FACE; ++x)
    {
      indexBuffer->add(face.mIndices[x]);
    }
  }
  
  std::pair<VertexBuffer, IndexBuffer> pair = std::make_pair(*vertexBuffer, *indexBuffer);
  return pair;
}




