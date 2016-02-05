/*
  Filename   : Vector3.h
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Vector3 Class
  Description: Header file for Vector3
*/

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef VECTOR3_H      
#define VECTOR3_H

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>



/************************************************************/

class Vector3
{
public:

  float x, y, z;

  Vector3 ();

  Vector3 (float xyz);

  Vector3 (float x, float y, float z);

  ~Vector3 ();

  void
  set (float x, float y, float z);

  void
  negate ();

  float
  dot (const Vector3& v) const;

  // Return angle b/w *this and "v"
  float
  angleBetween (const Vector3& v);

  Vector3
  cross (const Vector3& v) const;

  float
  length () const;

  void
  normalize ();


  Vector3&
  operator+= (const Vector3& v);

  Vector3&
  operator-= (const Vector3& v);

  Vector3&
  operator*= (float s);

  Vector3&
  operator/= (float s);

private:

};

Vector3
operator+ (const Vector3& v1, const Vector3& v2);

Vector3
operator- (const Vector3& v1, const Vector3& v2);

Vector3
operator* (float s, const Vector3& v);

Vector3
operator* (const Vector3& v, float s);

Vector3
operator/ (const Vector3& v, float s);

/************************************************************/

#endif
