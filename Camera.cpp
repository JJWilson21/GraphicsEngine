/*
  Author     : Jeremiah Wilson
  Course     : CSCI 375
  Assignment : Camera
  Description: Implementation of Camera class
*/   

/************************************************************/
// System includes

#include <cstdlib>
/************************************************************/
// Local includes

#include "Vector3.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "Transform.h"


/************************************************************/
Camera::Camera ()
{

}

Camera::Camera (Vector3 backVec, Vector3 eyePoint, Vector3 upVec, Vector3 rightVec)
  
{
  //Got a warning initializing these along with the others
  m_camera.setPosition (eyePoint);
  Math::Matrix3 camera (rightVec, upVec, backVec);
  m_camera.setOrientation (camera);
  m_startPointMatrix = m_camera.getOrientation ();
  m_startPoint = eyePoint;
}

Camera::~Camera ()
{

}

/************************************************************/
void
Camera::yaw (float angleDegrees)
{
  m_camera.yaw (angleDegrees);
}

void
Camera::pitch (float angleDegrees)
{
  m_camera.pitch (angleDegrees);
}

void
Camera::roll (float angleDegrees)
{
  m_camera.roll (angleDegrees);
}

void
Camera::moveLocalRight (float distance)
{
  m_camera.moveRight (distance);
}

void
Camera::moveLocalUp (float distance)
{
  m_camera.moveUp (distance);
}

void
Camera::moveLocalBack (float distance)
{
  m_camera.moveBack (distance);
}

void
Camera::setBackVec (const float x, const float y, const float z)
{
  Vector3 back (x, y, z);
  back.negate ();
  this->m_camera.setBack (back);
  this->m_startPointMatrix.setBack (back);
}

void
Camera::setBackVec (const Vector3 v)
{
  Vector3 vec (v);
  vec.negate ();
  this->m_camera.setBack (vec);
  this->m_startPointMatrix.setBack (vec);
}

void
Camera::setEyePoint (const float x, const float y, const float z)
{
  Vector3 eye (x, y, z);
  m_camera.setPosition (eye);
  if (first)
  {
  this->m_startPoint = eye;	
	first = false;
}
}

void
Camera::setEyePoint (const Vector3 v)
{
  m_camera.setPosition (v);
  if (first){
  this->m_startPoint.set (v.x, v.y, v.z);
	first = false;
}
}

void
Camera::setUpVec (const float x, const float y, const float z)
{
  Vector3 up (x, y, z);
  this->m_camera.setUp (up);
  this->m_startPointMatrix.setUp (up);
}

void
Camera::setUpVec (const Vector3 v)
{
  this->m_camera.setUp (v);
  this->m_startPointMatrix.setUp (v);
}

void
Camera::setRightVec (const Vector3 v)
{
  this->m_camera.setRight (v);
  this->m_startPointMatrix.setRight (v);
}

void
Camera::setRightVec (const float x, const float y, const float z)
{
  Vector3 right (x, y, z);
  this->m_camera.setRight (right);
  this->m_startPointMatrix.setRight (right);
}

Vector3
Camera::getRightVec ()
{
  return m_camera.getRight ();
}

Vector3
Camera::getBackVec ()
{
  return m_camera.getBack ();
}

Vector3
Camera::getEyePoint ()
{
  return m_camera.getPosition ();
}

Vector3
Camera::getUpVec ()
{
  return m_camera.getUp ();
}

Math::Transform
Camera::getViewMatrix ()
{		
  Math::Transform view;
  Vector3 at (m_camera.getBack ().x + m_camera.getPosition ().x, m_camera.getBack ().y + m_camera.getPosition ().y, m_camera.getBack ().z + m_camera.getPosition ().z);
  Vector3 eye (m_camera.getPosition ());
  Vector3 up (m_camera.getUp ());
  Vector3 back = eye - at;
  back.normalize ();
  Vector3 right = up.cross(back);
  right.normalize ();
  eye.negate ();
  Math::Matrix3 rotScale (right, up, back);
  rotScale.transpose ();
  view.setOrientation (rotScale);
  Vector3 translation =  rotScale * eye;
  //Vector3 translation = -eye;
  view.setPosition (translation);
  return view;
}



Math::Matrix4
Camera::getPrespectiveProjectionMatrix ()
{

  m_projectionMatrix.setToPerspectiveProjection(m_verticalFov, m_aspectRatio, m_nearZ, m_farZ);
  return m_projectionMatrix;
}

Math::Matrix4
Camera::getOrthographicProjectionMatrix ()
{
  m_projectionMatrix.setToOrthographicProjection(-right, right, -top, top, m_nearZ, m_farZ);
  return m_projectionMatrix;
}

void
Camera::reset ()
{
  setEyePoint (m_startPoint);
  m_camera.setRight (m_startPointMatrix.getRight ());
  m_camera.setUp (m_startPointMatrix.getUp ());
  m_camera.setBack (m_startPointMatrix.getBack ());
}

bool
Camera::isPrespective()
{
  return m_projectionMatrix.isPrespective();
}

void
Camera::zoom(float deltaFov)
{
  if (deltaFov > 0 && m_verticalFov > glm::radians(120.0f))
  {

  }
  else if (deltaFov < 0 && m_verticalFov < glm::radians(1.0f))
  {

  }
  else
  {
    m_verticalFov += deltaFov;
  }
}

void
Camera::resetAspectRatio(int width, int height)
{
  m_aspectRatio = (float) ((float)width / (float)height);
}
/************************************************************/




