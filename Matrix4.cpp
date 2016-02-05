/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Matrix4
  Description: Implementation of Matrix4 class
*/  
/************************************************************/
// System includes
#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
/************************************************************/
// Local includes
#include "Matrix4.h"


namespace Math
{
  // Represent a 4x4 matrix using column-major storage.
  // Matrix is interpreted thus:
  // [ m_00 m_10 m_20 m_30 ] 
  // [ m_01 m_11 m_21 m_31 ] 
  // [ m_02 m_12 m_22 m_32 ] 
  // [ m_03 m_13 m_23 m_33 ] 
  // Leave the matrix uninitialized
  Matrix4::Matrix4()
  {
  }

  // Initialize to identity regardless of the value of "makeIdentity"
                        
  Matrix4::Matrix4(bool makeIdentity)
  {
    Math::Matrix4 m;
    m.setToIdentity();
  }

  // Assume "arrayOf16" uses column-major storage. 
                

  Matrix4::Matrix4(const float arrayOf16[16])
  {
    m_matrix[0][0] = arrayOf16[0];
    m_matrix[0][1] = arrayOf16[1];
    m_matrix[0][2] = arrayOf16[2];
    m_matrix[0][3] = arrayOf16[3];
    m_matrix[1][0] = arrayOf16[4];
    m_matrix[1][1] = arrayOf16[5];
    m_matrix[1][2] = arrayOf16[6];
    m_matrix[1][3] = arrayOf16[7];
    m_matrix[2][0] = arrayOf16[8];
    m_matrix[2][1] = arrayOf16[9];
    m_matrix[2][2] = arrayOf16[10];
    m_matrix[2][3] = arrayOf16[11];
    m_matrix[3][0] = arrayOf16[12];
    m_matrix[3][1] = arrayOf16[13];
    m_matrix[3][2] = arrayOf16[14];
    m_matrix[3][3] = arrayOf16[15];
  }

  // Initialize upper 3x3 with "m", and other components to 0.
  // Put 1 in m_33.

  Matrix4::Matrix4(const Matrix3& m)
  {
    m_matrix[0][0] = m.getRight().x;
    m_matrix[0][1] = m.getRight().y;
    m_matrix[0][2] = m.getRight().z;
    m_matrix[0][3] = 0;
    m_matrix[1][0] = m.getUp().x;
    m_matrix[1][1] = m.getUp().y;
    m_matrix[1][2] = m.getUp().z;
    m_matrix[1][3] = 0;
    m_matrix[2][0] = m.getBack().x;
    m_matrix[2][1] = m.getBack().y;
    m_matrix[2][2] = m.getBack().z;
    m_matrix[2][3] = 0;
    m_matrix[3][0] = 0;
    m_matrix[3][1] = 0;
    m_matrix[3][2] = 0;
    m_matrix[3][3] = 1;
  }

  // Do same as constructor above. 
  void
  Matrix4::setFromMatrix(const Matrix3& m)
  {
    m_matrix[0][0] = m.getRight().x;
    m_matrix[0][1] = m.getRight().y;
    m_matrix[0][2] = m.getRight().z;
    m_matrix[0][3] = 0;
    m_matrix[1][0] = m.getUp().x;
    m_matrix[1][1] = m.getUp().y;
    m_matrix[1][2] = m.getUp().z;
    m_matrix[1][3] = 0;
    m_matrix[2][0] = m.getBack().x;
    m_matrix[2][1] = m.getBack().y;
    m_matrix[2][2] = m.getBack().z;
    m_matrix[2][3] = 0;
    m_matrix[3][0] = 0;
    m_matrix[3][1] = 0;
    m_matrix[3][2] = 0;
    m_matrix[3][3] = 1;
  }

  // Set to identity matrix. 
  void
  Matrix4::setToIdentity()
  {
    m_matrix[0][0] = 1;
    m_matrix[0][1] = 0;
    m_matrix[0][2] = 0;
    m_matrix[0][3] = 0;
    m_matrix[1][0] = 0;
    m_matrix[1][1] = 1;
    m_matrix[1][2] = 0;
    m_matrix[1][3] = 0;
    m_matrix[2][0] = 0;
    m_matrix[2][1] = 0;
    m_matrix[2][2] = 1;
    m_matrix[2][3] = 0;
    m_matrix[3][0] = 0;
    m_matrix[3][1] = 0;
    m_matrix[3][2] = 0;
    m_matrix[3][3] = 1;
  }

  // Set to zero matrix. 
  void
  Matrix4::setToZero()
  {
    for (int x = 0; x <= 3; ++x)
    {
      for (int y = 0; y <= 3; ++y)
      {
        m_matrix[x][y] = 0;
      }
    }
  }

  // Build a translation matrix.
  void
  Matrix4::setToTranslation(const Vector3& translation)
  {
    this->setToIdentity();
    this->m_matrix[3][0] = translation.x;
    this->m_matrix[3][1] = translation.y;
    this->m_matrix[3][2] = translation.z;
  }

  // Build a uniform scale matrix.
  void
  Matrix4::setToScale(float scale)
  {
    this->setToIdentity();
    this->m_matrix[0][0] = scale;
    this->m_matrix[1][1] = scale;
    this->m_matrix[2][2] = scale;
  }

  // Build a non-uniform scale matrix.
  void
  Matrix4::setToScale(float scaleX, float scaleY, float scaleZ)
  {
    this->setToIdentity();
    this->m_matrix[0][0] = scaleX;
    this->m_matrix[1][1] = scaleY;
    this->m_matrix[2][2] = scaleZ;
  }

  // Build rotation matrices. 
  void
  Matrix4::setToRotationX(float angleDegrees)
  {
    this->setToIdentity();
    this->m_matrix[1][1] = cos(angleDegrees);
    this->m_matrix[1][2] = -sin(angleDegrees);
    this->m_matrix[2][1] = sin(angleDegrees);
    this->m_matrix[2][2] = cos(angleDegrees);
  }

  void
  Matrix4::setToRotationY(float angleDegrees)
  {
    this->setToIdentity();
    this->m_matrix[0][0] = cos(angleDegrees);   
    this->m_matrix[0][2] = sin(angleDegrees);
    this->m_matrix[2][0] = -sin(angleDegrees);
    this->m_matrix[2][2] = cos(angleDegrees);
  }

  void
  Matrix4::setToRotationZ(float angleDegrees)
  {
    this->setToIdentity();
    this->m_matrix[0][0] = cos(angleDegrees);
    this->m_matrix[0][1] = -sin(angleDegrees);
    this->m_matrix[1][0] = sin(angleDegrees);
    this->m_matrix[1][1] = cos(angleDegrees);
  }

  // Build an arbitrary axis rotation matrix. 
  void
  Matrix4::setFrontAngleAxis(float angleDegrees, const Vector3& unitAxis)
  {
    float minor00 = unitAxis.x * unitAxis.x + cos(angleDegrees) * (1 - unitAxis.x * unitAxis.x);
    float minor01 = unitAxis.x * unitAxis.y * (1 - cos(angleDegrees)) + unitAxis.z * sin(angleDegrees);
    float minor02 = unitAxis.x * unitAxis.z * (1 - cos(angleDegrees)) + unitAxis.y * sin(angleDegrees);
    float minor10 = unitAxis.x * unitAxis.y * (1 - cos(angleDegrees)) + unitAxis.z * sin(angleDegrees);
    float minor11 = unitAxis.y * unitAxis.y + cos(angleDegrees) * (1 - unitAxis.y * unitAxis.y);
    float minor12 = unitAxis.y * unitAxis.z * (1 - cos(angleDegrees)) - unitAxis.x * sin(angleDegrees);
    float minor20 = unitAxis.x * unitAxis.z * (1 - cos(angleDegrees)) - unitAxis.y * sin(angleDegrees);
    float minor21 = unitAxis.y * unitAxis.z * (1 - cos(angleDegrees)) + unitAxis.x * sin(angleDegrees);
    float minor22 = unitAxis.z * unitAxis.z + cos(angleDegrees) * (1 - unitAxis.z * unitAxis.z);
    m_00 = minor00;
    m_01 = minor01;
    m_02 = minor02;
    m_03 = 0;
    m_10 = minor10;
    m_11 = minor11;
    m_12 = minor12;
    m_13 = 0;
    m_20 = minor20;
    m_21 = minor21;
    m_22 = minor22;
    m_23 = 0;
    m_30 = 0;
    m_31 = 0;
    m_32 = 0;
    m_33 = 1;   
  }

  // Assume "arrayOf16" uses column-major storage. 
  void
  Matrix4::getAsArray(float arrayOf16[16]) const
  {
    arrayOf16[0] = m_00;
    arrayOf16[1] = m_01;
    arrayOf16[2] = m_02;
    arrayOf16[3] = m_03;
    arrayOf16[4] = m_10;
    arrayOf16[5] = m_11;
    arrayOf16[6] = m_12;
    arrayOf16[7] = m_13;
    arrayOf16[8] = m_20;
    arrayOf16[9] = m_21;
    arrayOf16[10] = m_22;
    arrayOf16[11] = m_23;
    arrayOf16[12] = m_30;
    arrayOf16[13] = m_31;
    arrayOf16[14] = m_32;
    arrayOf16[15] = m_33;
  }

  // Return a pointer to the first element. 
  const float*
  Matrix4::data() const
  {
    return &m_00;
  }

  void
  Matrix4::transpose()
  {
    std::swap(m_01, m_10);      
    std::swap(m_02, m_20);
    std::swap(m_03, m_30);
    std::swap(m_13, m_31);
    std::swap(m_23, m_32);
  }

  void
  Matrix4::negate()
  {
    for (int x = 0; x <= 3; ++x)
    {
      for (int y = 0; y <= 3; ++y)
      {
        m_matrix[x][y] = -m_matrix[x][y];
      }
    }
  }

  void
  Matrix4::setToLookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
  {
    Vector3 back = eye - at;
    Vector3 right = back.cross(up);
    Math::Matrix3 m (right, up, back);
    this->setFromMatrix(m);
    m_30 = eye.x;
    m_31 = eye.y;
    m_32 = eye.z;
    m_33 = 1;
  }

  void
  Matrix4::setToPerspectiveProjection(double fovYDegrees, double aspectRatio,
                                      double nearPlaneZ, double farPlaneZ)
  {
    perspective = true;
    setToZero();
    float minor00 = 1.0f / (aspectRatio * tan(fovYDegrees / 2.0f));
    float minor11 = 1.0f / (tan(fovYDegrees / 2.0f));
    float minor22 = (nearPlaneZ + farPlaneZ) / (nearPlaneZ - farPlaneZ);
    float minor32 = (2.0f * nearPlaneZ * farPlaneZ) / (nearPlaneZ - farPlaneZ);
    m_00 = minor00;
    m_11 = minor11;
    m_22 = minor22;
    m_23 = -1;
    m_32 = minor32;
  }

  void
  Matrix4::setToPerspectiveProjection(double left, double right, double bottom,
                                      double top, double nearPlaneZ,
                                      double farPlaneZ)
  {
    perspective = true;
    float minor00 = (2.0f * nearPlaneZ) / (right - left);
    float minor02 = (right + left) / (right - left);
    float minor11 = (2.0f * nearPlaneZ) / (top - bottom);
    float minor12 = (top + bottom) / (top - bottom);
    float minor22 = -(farPlaneZ + nearPlaneZ) / (farPlaneZ - nearPlaneZ);
    float minor23 = (-2.0f * farPlaneZ * nearPlaneZ) / (farPlaneZ - nearPlaneZ);
    setToZero();
    m_00 = minor00;
    m_02 = minor02;
    m_11 = minor11;
    m_12 = minor12;
    m_22 = minor22;
    m_23 = minor23;
    m_32 = -1;
  }

  void
  Matrix4::setToSymmetricPerspectiveProjection(double right, double top,
                                               double nearPlaneZ,
                                               double farPlaneZ)
  {
    float left = -right;
    float bottom = -top;
    setToPerspectiveProjection(left, right, bottom, top, nearPlaneZ, farPlaneZ);
  }

  void
  Matrix4::setToOrthographicProjection(double left, double right, double bottom, double top, double nearPlaneZ, double farPlaneZ)
  {
    perspective = false;
    float minor00 = 2.0f / (right - left);
    float minor11 = 2.0f / (top - bottom);
    float minor22 = -2.0f / (farPlaneZ - nearPlaneZ);
    float minor30 = -((right + left) / (right - left));
    float minor31 = -((top + bottom) / (top - bottom));
    float minor32 = -((farPlaneZ + nearPlaneZ) / (farPlaneZ - nearPlaneZ));
    setToZero();
    m_00 = minor00;
    m_11 = minor11;
    m_22 = minor22;
    m_30 = minor30;
    m_31 = minor31;
    m_32 = minor32;
    m_33 = 1;
  }

  bool
  Matrix4::isPrespective()
  {
    return perspective;
  }
  // Operators
  Matrix4&
  Matrix4::operator+= (const Matrix4& m)
  {
    for (int x = 0; x <= 3; ++x)
    {
      for (int y = 0; y <= 3; ++y)
      {
        this->m_matrix[x][y] += m.m_matrix[x][y];
      }
    }

    return *this;
  }

  Matrix4&
  Matrix4::operator-= (const Matrix4& m)
  {
    for (int x = 0; x <= 3; ++x)
    {
      for (int y = 0; y <= 3; ++y)
      {
        this->m_matrix[x][y] -= m.m_matrix[x][y];
      }
    }

    return *this;
  }

  Matrix4&
  Matrix4::operator*= (float s)
  {
    for (int x = 0; x <= 3; ++x)
    {
      for (int y = 0; y <= 3; ++y)
      {
        this->m_matrix[x][y] *= s;
      }
    }

    return *this;
  }

  Matrix4&
  Matrix4::operator*= (const Matrix4& m)
  {
    float tempMatrix[16];
    tempMatrix[0] = (m_00 * m.m_00) + (m_10 * m.m_01) + (m_20 * m.m_02) + (m_30 * m.m_03);
    tempMatrix[1] = (m_01 * m.m_00) + (m_11 * m.m_01) + (m_21 * m.m_02) + (m_31 * m.m_03);
    tempMatrix[2] = (m_02 * m.m_00) + (m_12 * m.m_01) + (m_22 * m.m_02) + (m_32 * m.m_03);
    tempMatrix[3] = (m_03 * m.m_00) + (m_13 * m.m_01) + (m_23 * m.m_02) + (m_33 * m.m_03);
    tempMatrix[4] = (m_00 * m.m_10) + (m_10 * m.m_11) + (m_20 * m.m_12) + (m_30 * m.m_13);
    tempMatrix[5] = (m_01 * m.m_10) + (m_11 * m.m_11) + (m_21 * m.m_12) + (m_31 * m.m_13);
    tempMatrix[6] = (m_02 * m.m_10) + (m_12 * m.m_11) + (m_22 * m.m_12) + (m_32 * m.m_13);
    tempMatrix[7] = (m_03 * m.m_10) + (m_13 * m.m_11) + (m_23 * m.m_12) + (m_33 * m.m_13);
    tempMatrix[8] = (m_00 * m.m_20) + (m_10 * m.m_21) + (m_20 * m.m_22) + (m_30 * m.m_23);
    tempMatrix[9] = (m_01 * m.m_20) + (m_11 * m.m_21) + (m_21 * m.m_22) + (m_31 * m.m_23);
    tempMatrix[10] = (m_02 * m.m_20) + (m_12 * m.m_21) + (m_22 * m.m_22) + (m_32 * m.m_23);
    tempMatrix[11] = (m_03 * m.m_20) + (m_13 * m.m_21) + (m_23 * m.m_22) + (m_33 * m.m_23);
    tempMatrix[12] = (m_00 * m.m_30) + (m_10 * m.m_31) + (m_20 * m.m_32) + (m_30 * m.m_33);
    tempMatrix[13] = (m_01 * m.m_30) + (m_11 * m.m_31) + (m_21 * m.m_32) + (m_31 * m.m_33);
    tempMatrix[14] = (m_02 * m.m_30) + (m_12 * m.m_31) + (m_22 * m.m_32) + (m_32 * m.m_33);
    tempMatrix[15] = (m_03 * m.m_30) + (m_13 * m.m_31) + (m_23 * m.m_32) + (m_33 * m.m_33);
    Math::Matrix4 newMatrix(tempMatrix);
    *this = newMatrix;
    return *this;
  }



  Matrix4
  operator+ (const Matrix4& m1, const Matrix4& m2)
  {
    Math::Matrix4 m3(m1);
    return m3 += m2;
  }

  Matrix4
  operator- (const Matrix4& m1, const Matrix4& m2)
  {
    Math::Matrix4 m3(m1);
    return m3 -= m2;
  }

  // Unary negation
  Matrix4
  operator- (const Matrix4& m)
  {
    Math::Matrix4 m1(m);
    m1.negate();
    return m1;
  }

  Matrix4
  operator* (const Matrix4& m1, const Matrix4& m2)
  {
    Math::Matrix4 m3(m1);
    return m3 *= m2;
  }

  std::ostream&
  operator<< (std::ostream& outStream, const Matrix4& m)
  {     
    float matrixArray[16];
    m.getAsArray(matrixArray);
    std::string top4 = std::string("[ ") + std::to_string(matrixArray[0]) + " "
      + std::to_string(matrixArray[4]) + " " + std::to_string(matrixArray[8]) + " " + std::to_string(matrixArray[12]) + " ]\n";

    std::string topMiddle4 = std::string("[ ") + std::to_string(matrixArray[1]) + " "
      + std::to_string(matrixArray[5]) + " " + std::to_string(matrixArray[9]) + " " + std::to_string(matrixArray[13]) + " ]\n";

    std::string bottomMiddle4 = std::string("[ ") + std::to_string(matrixArray[2]) + " "
      + std::to_string(matrixArray[6]) + " " + std::to_string(matrixArray[10]) + " " + std::to_string(matrixArray[14]) + " ]\n";

    std::string bottom4 = std::string("[ ") + std::to_string(matrixArray[3]) + " " + std::to_string(matrixArray[7]) + " " + std::to_string(matrixArray[11]) + " " + std::to_string(matrixArray[15]) + " ]\n";
    outStream << std::setprecision(2);
    outStream << top4 << topMiddle4 << bottomMiddle4 << bottom4;
    return outStream;
  }
}


