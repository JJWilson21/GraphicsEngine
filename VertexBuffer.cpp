
/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : VertexBuffer
  Description: Implementation of VertexBuffer class
*/
/************************************************************/
// System Includes
#include <iostream>
#include <string>

/************************************************************/
// Local Includes
#include "VertexBuffer.h"




VertexBuffer::VertexBuffer()
{

}

void
VertexBuffer::addData(const aiVector3D& data, const int count)
{
  for (int x = 0; x < count; ++x)
  {
    m_vertexData.push_back(data[x]);
  }
}

void
VertexBuffer::setVertexSizeFloats(int size)
{
  m_vertexFloatCount = size;
}

int
VertexBuffer::getVertexSizeFloats()
{
  return m_vertexFloatCount;
}

int
VertexBuffer::size()
{
  return m_vertexData.size();
}

float&
VertexBuffer::front()
{
  return m_vertexData.front();
}
