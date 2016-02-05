

/*
Author     : Jeremiah Wilson
Course     : CSCI 375
Assignment : Camera
Description: Implementation of Camera class
*/
/************************************************************/
// System includes
#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
/************************************************************/
// Local includes
#include "Vector3.h"
#include "Matrix3.h"

	// Basis vectors are stored in Vector3's and form
	//   the columns of a 3x3 matrix. 
	// Matrix is interpreted thus:
	// [ rx ux bx ]
	// [ ry uy by ]
	// [ rz uz bz ]
	// Operations are consistent with column vectors (v' = M * v).
		// Initialize to identity. 
namespace Math{

Matrix3::Matrix3 ()
{

}

// Initial right vector to (rz, ry, rz), etc.
Matrix3::Matrix3 (float rx, float ry, float rz,
float ux, float uy, float uz,
float bx, float by, float bz)
	:m_right (rx, ry, rz), m_up (ux, uy, uz), m_back (bx, by, bz)
{

}

		// Initialize corresponding basis vectors
Matrix3::Matrix3 (const Vector3& right, const Vector3& up,
const Vector3& back)
	:m_right (right), m_up (up), m_back (back)
{

}

		// Initialize right, up, and back. Compute back.
		// If "makeOrthonormal" is true, ensure each vector is of
		//   unit length and all 3 vectors are mutually perpendicular.
Matrix3::Matrix3 (const Vector3& right, const Vector3& up,
bool makeOrthonormal)
  :m_right (right), m_up (up)
{
  if (makeOrthonormal)
  {
    this->orthonormalize ();
  }
  
}


		// Set to identity matrix.
void
Matrix3::setToIdentity ()
{
  this->m_right.set (1, 0, 0);
  this->m_up.set (0, 1, 0);
  this->m_back.set (0, 0, 1);
}

		// Set all elements to 0.
void
Matrix3::setToZero ()
{
  this->m_right.set (0, 0, 0);
  this->m_up.set (0, 0, 0);
  this->m_back.set (0, 0, 0);
}

		// Return a pointer to the first element
const float*
Matrix3::data () const
{
  return &this->m_right.x;
}

		// Set the right vector.
void
Matrix3::setRight (const Vector3& right)
{
  this->m_right = right;
}

		// Get the right vector.
Vector3
Matrix3::getRight () const
{
  return m_right;
}

void
Matrix3::setUp (const Vector3& up)
{
  this->m_up = up;
}

Vector3
Matrix3::getUp () const
{
  return m_up;
}

void
Matrix3::setBack (const Vector3& back)
{
  this->m_back = back;
}

Vector3
Matrix3::getBack () const
{
  return m_back;
}

		// Set the back vector to the negation of "forward".
void
Matrix3::setForward (const Vector3& forward)
{
  this->m_back = forward;
  m_back.negate ();
}

Vector3
Matrix3::getForward()
{
	
  m_back.negate ();
  return m_back;
}

		// Invert a rotation matrix.
void
Matrix3::invertRotation ()
{
  this->transpose ();
}

		// Invert a general matrix.
void
Matrix3::invert ()
{
  //having trouble with this
	if (this->determinant () == 0)
	{
		return;
	}
    float inverseDeterminant = 1.0f / this->determinant ();
	float minor11 = m_up.y * m_back.z - m_up.z * m_back.y;
	float minor12 = m_up.z * m_back.x - m_up.x * m_back.z;
	float minor13 = m_up.x * m_back.y - m_up.y * m_back.x;
	float minor21 = m_right.z * m_back.y - m_right.y * m_back.z;
	float minor22 = m_right.x * m_back.z - m_right.z * m_back.x;
	float minor23 = m_right.y * m_back.x - m_right.x * m_back.y;
	float minor31 = m_right.y * m_up.z - m_right.z * m_up.y;
	float minor32 = m_right.z * m_up.x - m_right.x * m_up.z;
	float minor33 = m_right.x * m_up.y - m_right.y * m_up.x;
	Vector3 right (minor11, minor12, minor13);
	right /= inverseDeterminant;
	Vector3 up (minor21, minor22, minor23);
	up /= inverseDeterminant;
	Vector3 back (minor31, minor32, minor33);
	back /= inverseDeterminant;
	this->setRight (right);
	this->setUp (up);
	this->setBack (back);
}

float
Matrix3::determinant () const
{
	float first = m_right.x * (m_up.y * m_back.z - m_up.z * m_back.y);
	float second = m_right.y * (m_up.x * m_back.z - m_up.z * m_back.x);
	float third = m_right.z * (m_up.x * m_back.y - m_up.y * m_back.x);
	return first - second + third;
}


void
Matrix3::transpose ()
{
	
	std::swap (m_up.x, m_right.y);
	std::swap (m_back.x, m_right.z);
	std::swap (m_back.y, m_up.z);

}

		// Ensure all axes are of unit length and mutually perpendicular.
void
Matrix3::orthonormalize ()
{
  m_back.normalize ();
  m_right = m_up.cross (m_back);
  m_right.normalize ();
  m_up = m_back.cross  (m_right);
}

		// Set to a uniform scale matrix.
void
Matrix3::setToScale (float scale)
{
	this->m_right.set (scale, 0, 0);
	this->m_up.set (0, scale, 0);
	this->m_back.set (0, 0, scale);
}

		// Set to a non-uniform scale matrix.
void
Matrix3::setToScale (float scaleX, float scaleY, float scaleZ)
{
	this->m_right.set (scaleX, 0, 0); 
	this->m_up.set (0, scaleY, 0);
	this->m_back.set (0, 0, scaleZ);
}

		// Set to a shear matrix that shears x by y and z.
void
Matrix3::setToShearXByYz (float shearY, float shearZ)
{
	this->m_right.set (1, 0, 0);
	this->m_up.set (shearY, 1, 0);
	this->m_back.set (shearZ, 0, 1);
}

		// Set to a shear matrix that shears y by x and z.
void
Matrix3::setToShearYByXz (float shearX, float shearZ)
{
	this->m_right.set (1, shearX, 0);
	this->m_up.set (0, 1, 0);
	this->m_back.set (0, shearZ, 1);
}

		// Set to a shear matrix that shears z by x and y.
void
Matrix3::setToShearZByXy (float shearX, float shearY)
{
	this->m_right.set (1, 0, shearX);
	this->m_up.set (0, 1, shearY);
	this->m_back.set (0, 0, 1);
}


		// Set to an x-axis rotation matrix.
void
Matrix3::setToRotationX (float angleDegrees)
{
	float angleRadians = (angleDegrees* 3.14159f) / 180.0f;
	this->m_right.set (1, 0, 0);
	this->m_up.set(0, cos(angleRadians), sin(angleRadians));
	this->m_back.set (0, -sin (angleRadians), cos (angleRadians));
}

void
Matrix3::setToRotationY (float angleDegrees)
{
	float angleRadians = (angleDegrees* 3.14159f) / 180.0f;
	this->m_right.set (cos (angleRadians), 0, -sin (angleRadians));
	this->m_up.set (0, 1, 0);
	this->m_back.set (sin (angleRadians), 0, cos (angleRadians));
}

void
Matrix3::setToRotationZ (float angleDegrees)
{
	float angleRadians = (angleDegrees* 3.14159f) / 180.0f;
	this->m_right.set (cos (angleRadians), sin (angleRadians), 0);
	this->m_up.set(-sin (angleRadians), cos (angleRadians), 0);
	this->m_back.set(0, 0, 1);
}

		// Set to a general rotation matrix about "axis". 
void
Matrix3::setFromAngleAxis (float angleDegrees, const Vector3& axis)
{
  
  this->m_right.set (cos (angleDegrees) + axis.x * axis.x * (1- cos (angleDegrees)), axis.x * axis.y * (1 - cos (angleDegrees)) - axis.z * sin (angleDegrees), axis.x * axis.z * (1 - cos (angleDegrees)) + axis.y * sin (angleDegrees));
  
  this->m_up.set(axis.y * axis.x * (1 - cos (angleDegrees)) + axis.z * sin (angleDegrees),
	      cos (angleDegrees) + axis.y * axis.y * (1 - cos (angleDegrees)),
		 axis.y * axis.z * (1 - cos (angleDegrees)) - axis.x * (sin (angleDegrees)));
  
  this->m_back.set(axis.z * axis.x * (1 - cos (angleDegrees)) - axis.y * sin (angleDegrees),
		   axis.z * axis.y * (1 - cos (angleDegrees)) + axis.x * sin (angleDegrees),
		   cos (angleDegrees) + axis.z * axis.z * (1 - cos (angleDegrees)));
	       
}

		// Negate all entries.
void
Matrix3::negate ()
{
	this->m_right.negate ();
	this->m_up.negate ();
	this->m_back.negate ();
}

		// Compute *this* point.
Vector3
Matrix3::transform (const Vector3& point) const
{
  return (*this * point);
}

Matrix3&
Matrix3::operator+= (const Matrix3& m)
{
	this->m_right += m.m_right;
	this->m_up += m.m_up;
	this->m_back += m.m_back;
	return *this;
}

Matrix3&
Matrix3::operator-= (const Matrix3& m)
{
	this->m_right -= m.m_right;
	this->m_up -= m.m_up;
	this->m_back -= m.m_back;
	return *this;
}

Matrix3&
Matrix3::operator*= (float scalar)
{
	m_right *= scalar;
	m_up *= scalar;
	m_back *= scalar;
	return *this;
}

		// Matrix multiply.
Matrix3&
Matrix3::operator*= (const Matrix3& m)
{
	
	Matrix3 m1 (*this);
	Vector3 row1 (m1.m_right.x, m1.m_up.x, m1.m_back.x);
	Vector3 row2 (m1.m_right.y, m1.m_up.y, m1.m_back.y);
	Vector3 row3 (m1.m_right.z, m1.m_up.z, m1.m_back.z);
	this->m_right.set (row1.dot(m.m_right), row2.dot(m.m_right), row3.dot(m.m_right));
	this->m_up.set(row1.dot(m.m_up), row2.dot(m.m_up), row3.dot(m.m_up));
	this->m_back.set(row1.dot(m.m_back), row2.dot(m.m_back), row3.dot(m.m_back));
	return *this;

}



Matrix3
operator+ (const Matrix3& m1, const Matrix3& m2)
{
	Matrix3 m3 (m1);
	return m3 += m2;
}

Matrix3
operator- (const Matrix3& m1, const Matrix3& m2)
{
	Matrix3 m3 (m1);
	return m3 -= m2;
}


	// Unary negation
Matrix3
operator- (const Matrix3& m)
{
	Matrix3 m1 (m);
	m1.negate ();
	return m1;
}

Matrix3
operator* (const Matrix3& m, float scalar)
{
	Matrix3 m1 (m);
	return m1*= scalar;
}

Matrix3
operator* (float scalar, const Matrix3& m)
{
	Matrix3 m1 (m);
	return m1*= scalar;
}

Matrix3
operator* (const Matrix3& m1, const Matrix3& m2)
{
	Matrix3 m3 (m1);
	return m3 *= m2;
}

Vector3
operator* (const Matrix3& m, const Vector3& v)
{
        Vector3 row1 (m.getRight().x, m.getUp().x, m.getBack().x);
	Vector3 row2 (m.getRight().y, m.getUp().y, m.getBack().y);
	Vector3 row3 (m.getRight().z, m.getUp().z, m.getBack().z);
	float m1 = row1.dot(v);
	float m2 = row2.dot(v);
	float m3 = row3.dot(v);
	Vector3 v1(m1, m2, m3);
	return (v1);
}


	// Print "m" in a neat, tabular format.
std::ostream&
operator<< (std::ostream& outStream, const Matrix3& m)
{
	std::string topRow = std::string ("[ ") + std::to_string (m.getRight().x) + " "
	  + std::to_string (m.getUp ().x) + " " + std::to_string (m.getBack ().x) + " ]\n";
	
	std::string middleRow = std::string ("[ ") + std::to_string (m.getRight ().y) + " "
	  + std::to_string (m.getUp ().y) + " " + std::to_string (m.getBack ().y) + " ]\n";
	
	std::string bottomRow = std::string("[ ") + std::to_string (m.getRight ().z) + " "
	  + std::to_string (m.getUp ().z) + " " + std::to_string (m.getBack ().z) + " ]\n";
	outStream << std::setprecision (2);
	outStream << topRow << middleRow << bottomRow;
	return outStream;
}

}

