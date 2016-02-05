/*
Filename   : Material.h
Author     : Jeremiah Wilson
Course     : CSCI 375
Assignment : Material Class
Description: Header file for Material
*/   

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef MATERIAL_H    
#define MATERIAL_H

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>

/************************************************************/
// Local includes

#include "Vector3.h"



/************************************************************/

class Material
{
  public:

	//  Material();
	
	//  ~Material();

  struct mat
  {
    Vector3 ambientReflection;
    Vector3 diffuseReflection;
    Vector3 specularReflection;
    float shininess;
  };
   
	mat mat;
  private:


};

/************************************************************/

#endif
