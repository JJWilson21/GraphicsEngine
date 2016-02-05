/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Camera
  Description: Implementation of Keybuffer class
*/   

/************************************************************/
// System includes


/************************************************************/
// Local includes
#include "KeyBuffer.h"

/************************************************************/
KeyBuffer::KeyBuffer ()
{

}

KeyBuffer::~KeyBuffer ()
{

}

void
KeyBuffer::setKeyUp (int key)
{
  keys[key] = 0;
}

void
KeyBuffer::setKeyDown (int key)
{
  keys[key] = 1;
}

bool
KeyBuffer::isDown (int key)
{
  return keys[key];
}

/************************************************************/


/************************************************************/




