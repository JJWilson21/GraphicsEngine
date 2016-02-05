
/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : VertexBuffer
  Description: Header file for VertexBuffer
*/   
/************************************************************/
// Macro guard
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
/************************************************************/
//System Includes
#include <string>
#include <memory>
#include <vector>
#include <assimp/scene.h>



class VertexBuffer
{

public:

  VertexBuffer();

  void
  addData(const aiVector3D& data, const int count);

  void
  setVertexSizeFloats(int size);
  
  int
  getVertexSizeFloats();

  int
  size();

  float&
  front();

  int m_positionFloatCount = 3;

  int m_normalFloatCount = 3;

  int m_textureFloatCount = 2;

        
  
private:
    
  std::vector<float> m_vertexData;
        
  int m_vertexFloatCount = 8;

        
        
};

#endif
