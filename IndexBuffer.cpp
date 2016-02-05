

/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : IndexBuffer.cpp
  Description: Implementation of IndexBuffer class
*/

/************************************************************/
// System includes

#include <iostream>
#include <string>

/************************************************************/
// Local includes
#include "IndexBuffer.h"


IndexBuffer::IndexBuffer()
{

}

IndexBuffer::~IndexBuffer()
{

}

void
IndexBuffer::add(GLuint index)
{
	indicies.push_back(index);
}

int
IndexBuffer::size()
{
	return indicies.size();
}

unsigned&
IndexBuffer::front()
{
	return indicies.front();
}



