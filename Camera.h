/*
Filename   : Camera.h
Author     : Jeremiah Wilson
Course     : CSCI 375
Assignment : Camera Class
Description: Header file for Camera
*/   

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef CAMERA_H       
#define CAMERA_H

/************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>

/************************************************************/
// Local includes

#include "Vector3.h"
#include "Matrix3.h"
#include "Transform.h"
#include "Matrix4.h"


/************************************************************/
// Using declarations

// Generally, avoid these in header files
// using std::ostream;

/************************************************************/

class Camera
{
  public:

    Camera ();

    Camera (Vector3 backVec, Vector3 eyePoint, Vector3 upVec, Vector3 rightVec);

    ~Camera ();

    void
    yaw (float angleDegrees);

    void
    pitch (float angleDegrees);

    void
    roll (float angleDegrees);

    void
    moveLocalRight (float distance);

    void
    moveLocalUp (float distance);

    void
    moveLocalBack (float distance);

    void
    setBackVec (const float x, const float y, const float z);

    void
    setBackVec (const Vector3 v);

    void
    setEyePoint (const float x, const float y, const float z);

    void
    setEyePoint (const Vector3 v);

    void
    setUpVec (const float x, const float y, const float z);

    void
    setUpVec (const Vector3 v);

    void
    setRightVec (const Vector3 v);

    void
    setRightVec (const float x, const float y, const float z);

    Vector3
    getRightVec ();

    Vector3
    getBackVec ();

    Vector3
    getEyePoint ();

    Vector3
    getUpVec ();

    Math::Transform
    getViewMatrix ();

    Math::Matrix4
    getPrespectiveProjectionMatrix ();

    Math::Matrix4
    getOrthographicProjectionMatrix ();

    void
    reset ();

    bool
    isPrespective();

    void
    zoom(float deltaFov);

    void
    resetAspectRatio(int width, int height);

  private:

    Vector3 m_backVec, m_upVec, m_projection, m_rightVec, m_startPoint;

    Math::Matrix3 m_startPointMatrix;

    Math::Transform m_camera;

    float m_verticalFov = glm::radians (50.0f);

    float m_aspectRatio = 4.0f / 3;

    const float right = 10;

    const float top = 10;

	bool first = true;

    // Near plane
    const float m_nearZ = 0.01f;
    // Far plane
    const float m_farZ = 200000.00f;

    Math::Matrix4 m_projectionMatrix;

};

/************************************************************/

#endif
