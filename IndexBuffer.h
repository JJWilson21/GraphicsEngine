/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : IndexBuffer
  Description: Header of IndexBuffer class
*/   
/************************************************************/
// Macro guard
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

/************************************************************/
// System includes
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IndexBuffer
{

public:

  IndexBuffer();

  ~IndexBuffer();

  void
  add(GLuint index);

  int
  size();

  unsigned&
  front();

        
  
private:
    
  std::vector<GLuint> indicies;
        
};

#endif
