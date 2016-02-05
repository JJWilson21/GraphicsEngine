/*
Filename   : Mesh.h
Author     : Jeremiah Wilson
Course     : CSCI 375
Description: Header file for KeyBuffer
*/

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef KEYBUFFER_H  
#define KEYBUFFER_H

/************************************************************/
// System includes

#include <bitset>
#include "GLFW/glfw3.h"
/************************************************************/
// Local includes

/************************************************************/

/************************************************************/


class KeyBuffer
{
  public:

    KeyBuffer ();

    ~KeyBuffer ();
	
    void
    setKeyUp (int key);

    void
    setKeyDown (int key);

    bool
    isDown (int key);


  private:

    std::bitset <GLFW_KEY_LAST + 1 > keys;

};

/************************************************************/

#endif
