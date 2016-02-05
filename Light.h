/*
Filename   : Light.h
Author     : Jeremiah Wilson
Course     : CSCI 375
Assignment : Light Class
Description: Header file for Light
*/   

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef LIGHT_H      
#define LIGHT_H

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>

/************************************************************/
// Local includes

#include "Vector3.h"



/************************************************************/

class Light
{
  public:

  struct light
  {
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    Vector3 direction;
  };
   
light l;

  private:


};

/************************************************************/

#endif
