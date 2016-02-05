/*
  Filename   : Vector3.cpp
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Vector3 Class
  Description: Implementation for Vector3
*/

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <cmath>

/************************************************************/
// Local includes

#include "Vector3.h"


/************************************************************/

Vector3::Vector3 ()
{

}

Vector3::Vector3 (float xyz)
{
  set (xyz, xyz, xyz);
}

Vector3::Vector3 (float x, float y, float z)
{
  set (x, y, z);
}

Vector3::~Vector3 ()
{
        
}

void
Vector3::set (float x, float y, float z)
{
  this->x = x;
  this->y = y;
  this->z = z;

}

void
Vector3::negate ()
{
  set (-this->x, -this->y, -this->z);
}

float
Vector3::dot (const Vector3& v) const
{
  float sum = 0;
  sum += this->x * v.x;
  sum += this->y * v.y;
  sum += this->z * v.z;
  return sum;

}

float
Vector3::angleBetween (const Vector3& v)
{       
  Vector3 vec (v);
  this->normalize ();
  vec.normalize ();
  return  (acos (this->dot (vec)) * 180) / 3.14159265359f;
}

float
Vector3::length () const
{
  return sqrt  ( (x*x) + (y*y) + (z*z) );
}

void
Vector3::normalize ()
{
  float length = this->length ();
  this->x /= length;
  this->y /= length;
  this->z /= length;

}

Vector3
Vector3::cross (const Vector3& v) const
{
  Vector3 vector;
  vector.x =  (this->y * v.z) -  (this->z * v.y);
  vector.y =  (this->z * v.x) -  (this->x * v.z);
  vector.z =  (this->x * v.y) -  (this->y * v.x);
  return vector;

}

Vector3&
Vector3::operator+= (const Vector3& v)
{
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector3&
Vector3::operator-= (const Vector3& v)
{
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector3&
Vector3::operator*= (float s)
{
  x *= s;
  y *= s;
  z *= s;
  return *this;
}

Vector3&
Vector3::operator/= (float s)
{
  x /= s;
  y /= s;
  z /= s;
  return *this;
}

Vector3
operator+ (const Vector3& v1, const Vector3& v2)
{
  Vector3 v (v1);
  v += v2;
  return v;
}

Vector3
operator- (const Vector3& v1, const Vector3& v2)
{
  Vector3 v (v1);
  v -= v2;
  return v;
}

Vector3
operator* (float s, const Vector3& v)
{
  Vector3 v1 (v);
  return v1 *= s;
}

Vector3
operator* (const Vector3& v, float s)
{
  Vector3 v1 (v);
  return v1 *= s;
}

Vector3
operator/ (const Vector3& v, float s)
{
  Vector3 v1 (v);
  return v1 /= s;
}
