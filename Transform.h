/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Transform
  Description: Header file for Transform class
*/   

/************************************************************/
// Macro Guard
#ifndef TRANSFORM_H
#define TRANSFORM_H

/************************************************************/
// Local Includes
#include "Matrix3.h"

#include "Vector3.h"

namespace Math
{
  class Transform
  {
  public:
    // Initialize to identity transform
    Transform ();

    ~Transform ();

    // Make right, up, and back orthonormal
    void
    orthonormalize ();

    // Reset to identity
    void
    reset ();

    void
    getTransform (float array[16]);

    Math::Transform
    combine(const Math::Transform& toCombine);

    Vector3
    getPosition () const;

    void
    setPosition (const Vector3& position);

    void
    setPosition (float positionX, float positionY, float positionZ);

    Vector3
    getRight () const;

    void
    setRight (const Vector3& right);

    Vector3
    getUp () const;

    void
    setUp (const Vector3& up);

    Vector3
    getBack () const;

    void
    setBack (const Vector3& back);

    void
    moveRight (float distance);

    void
    moveUp (float distance);

    void
    moveBack (float distance);

    void
    moveLocal (float distance, const Vector3& localDirection);

    void
    moveWorld (float distance, const Vector3& worldDirection);

    // Return the orientation and scale matrix
    Matrix3
    getOrientation () const;

    void
    setOrientation (const Matrix3& orientation);

    void
    setOrientation (const Vector3& right, const Vector3& up,
                    const Vector3& backward);

    // Rotate about the local X axis
    void
    pitch (float angleDegrees);

    // Rotate about the local Y axis
    void
    yaw (float angleDegrees);

    // Rotate about the local Z axis
    void
    roll (float angleDegrees);

    // Rotate about an arbitrary local axis
    void
    rotateLocal (float angleDegrees, const Vector3& axis);

    // Set "up" to world Y, and adjust "back" and "right"
    //   to ensure matrix is orthonormal
    void
    alignWithWorldY ();

    // Rotate around the world axis "axis"
    void
    rotateWorld (float angleDegrees, const Vector3& axis);

    // Scale locally using a uniform scale
    void
    scaleLocal (float scale);

    // Scale locally using a non-uniform scale
    void
    scaleLocal (float scaleX, float scaleY, float scaleZ);

    
    // Scale wrt world using a uniform scale
    void
    scaleWorld (float scale);

    // Scale wrt world using a non-uniform scale
    void
    scaleWorld (float scaleX, float scaleY, float scaleZ);

    // Shear locally
    void
    shearLocalXByYz (float shearY, float shearZ);

    void
    shearLocalYByXz (float shearX, float shearZ);

    void
    shearLocalZByXy (float shearX, float shearY);

  private:
    Matrix3 m_rotScale;
    Vector3 m_position;
  };
}

#endif
