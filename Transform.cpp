/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Transform Class
  Description: Implementation of Transform Class
*/

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
/************************************************************/
// Local includes

#include "Matrix3.h"

#include "Vector3.h"
#include "Transform.h"

/************************************************************/
namespace Math {

  Transform::Transform ()
  {

  }

  Transform::~Transform ()
  {

  }


  void
  Transform::orthonormalize ()
  {
    m_rotScale.orthonormalize ();
  }

  void
  Transform::reset ()
  {
    m_rotScale.setToIdentity ();
    m_position.set (0,0,0);
  }

  void
  Transform::getTransform (float array[16])
  {     
    array[0] = m_rotScale.getRight ().x;
    array[1] = m_rotScale.getRight ().y;
    array[2] = m_rotScale.getRight ().z;
    array[3] = 0;
    array[4] = m_rotScale.getUp ().x;
    array[5] = m_rotScale.getUp ().y;
    array[6] = m_rotScale.getUp ().z;
    array[7] = 0;
    array[8] = m_rotScale.getBack ().x;
    array[9] = m_rotScale.getBack ().y;
    array[10] = m_rotScale.getBack ().z;
    array[11] = 0;
    array[12] = m_position.x;
    array[13] = m_position.y;
    array[14] = m_position.z;
    array[15] = 1;

  }


  Math::Transform
  Transform::combine (const Math::Transform& toCombine)
  {
    Math::Matrix3 newRotScale = this->m_rotScale * toCombine.m_rotScale;
    Vector3 newPosition = this->m_rotScale * toCombine.m_position + this->m_position;
    this->m_rotScale = newRotScale;
    this->m_position = newPosition;
    return *this;
  }

  Vector3
  Transform::getPosition () const
  {
    return m_position;
  }

  void
  Transform::setPosition (const Vector3& position)
  {
    m_position.set (position.x, position.y, position.z);
  }

  void
  Transform::setPosition (float positionX, float positionY, float positionZ)
  {
    m_position.set (positionX, positionY, positionZ);
  }

  Vector3
  Transform::getRight () const
  {
    return m_rotScale.getRight ();
  }

  void
  Transform::setRight (const Vector3& right)
  {
    m_rotScale.setRight (right);
  }

  Vector3
  Transform::getUp () const
  {
    return m_rotScale.getUp ();
  }

  void
  Transform::setUp (const Vector3& up)
  {
    m_rotScale.setUp (up);
  }

  Vector3
  Transform::getBack () const
  {
    return m_rotScale.getBack ();
  }

  void
  Transform::setBack (const Vector3& back)
  {
    m_rotScale.setBack (back);
  }

  void
  Transform::moveRight (float distance)
  {
    this->m_position +=  (distance * this->getRight ());
  }

  void
  Transform::moveUp (float distance)
  {
    this->m_position +=  (distance * this->getUp ());
  }

  void
  Transform::moveBack (float distance)
  {
    this->m_position +=  (distance * this->getBack ());
  }

  void
  Transform::moveLocal (float distance, const Vector3& localDirection)
  {
    Vector3 worldDirection = m_rotScale.transform(localDirection);
    this->m_position +=  (distance * worldDirection);
  }

  void
  Transform::moveWorld (float distance, const Vector3& worldDirection)
  {
    this->m_position += (distance * worldDirection);
  }

  // Return the orientation and scale matrix
  Matrix3
  Transform::getOrientation () const
  {
    return m_rotScale;
  }

  void
  Transform::setOrientation (const Matrix3& orientation)
  {
    m_rotScale = orientation;
  }

  void
  Transform::setOrientation (const Vector3& right, const Vector3& up,
                             const Vector3& backward)
  {
    m_rotScale.setRight (right);
    m_rotScale.setUp (up);
    m_rotScale.setBack (backward);
  }

  // Rotate about the local X axis
  void
  Transform::pitch (float angleDegrees)
  {
    Math::Matrix3 m;
    m.setToRotationX (angleDegrees);
    this->m_rotScale *= m;
  }

  // Rotate about the local Y axis
  void
  Transform::yaw (float angleDegrees)
  {
    Math::Matrix3 m;
    m.setToRotationY (angleDegrees);
    this->m_rotScale *= m;
  }

  // Rotate about the local Z axis
  void
  Transform::roll (float angleDegrees)
  {
    Math::Matrix3 m;
    m.setToRotationZ (angleDegrees);
    this->m_rotScale *= m;
  }

  // Rotate about an arbitrary local axis
  void
  Transform::rotateLocal (float angleDegrees, const Vector3& axis)
  {
    Matrix3 m;
    m.setFromAngleAxis (angleDegrees, axis);
    this->m_rotScale = m * this->m_rotScale;;
  }

  // Set "up" to world Y, and adjust "back" and "right"
  //   to ensure matrix is orthonormal
  void
  Transform::alignWithWorldY ()
  {
    Vector3 newUp  (0, 1, 0);
    m_rotScale.setUp  (newUp);
    Vector3 newRight (m_rotScale.getUp ().cross (m_rotScale.getBack ()));
    newRight.normalize ();
    m_rotScale.setRight (newRight);
    m_rotScale.setBack (m_rotScale.getRight ().cross (m_rotScale.getUp ()));
  }

  // Rotate around the world axis "axis"
  void
  Transform::rotateWorld (float angleDegrees, const Vector3& axis)
  {
    Matrix3 m;
    m.setFromAngleAxis (angleDegrees, axis);
    this->m_rotScale = m * this->m_rotScale;
  }

  // Scale locally using a uniform scale
  void
  Transform::scaleLocal (float scale)
  {
    Matrix3 m;
    m.setToScale (scale);
    this->m_rotScale *= m;
  }

  // Scale locally using a non-uniform scale
  void
  Transform::scaleLocal (float scaleX, float scaleY, float scaleZ)
  {
    Matrix3 m;
    m.setToScale (scaleX, scaleY, scaleZ);
    this->m_rotScale *= m;
  }


  // Scale wrt world using a uniform scale
  void
  Transform::scaleWorld (float scale)
  {
    Matrix3 m;
    m.setToScale (scale);
    this->m_rotScale = m * this->m_rotScale;
    this->m_position = m * this->m_position;
  }

  // Scale wrt world using a non-uniform scale
  void
  Transform::scaleWorld (float scaleX, float scaleY, float scaleZ)
  {
    Matrix3 m;
    m.setToScale (scaleX, scaleY, scaleZ);
    this->m_rotScale = m * this->m_rotScale;
    this->m_position = m * this->m_position;
  }

  // Shear locally
  void
  Transform::shearLocalXByYz (float shearY, float shearZ)
  {
    Matrix3 m;
    m.setToShearXByYz (shearY, shearZ);
    this->m_rotScale *= m;
  }

  void
  Transform::shearLocalYByXz (float shearX, float shearZ)
  {
    Matrix3 m;
    m.setToShearYByXz (shearX, shearZ);
    this->m_rotScale *= m;
  }

  void
  Transform::shearLocalZByXy (float shearX, float shearY)
  {
    Matrix3 m;
    m.setToShearZByXy (shearX, shearY);
    this->m_rotScale *= m;
  }
}
