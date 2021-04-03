#include "camera.h"

Camera::Camera(Vec3f& _center, Vec3f& _dir, Vec3f& _up,float _angle):angle(_angle),center(_center),direction(_dir),up(_up)
{
	direction.Normalize();
	Vec3f::Cross3(horizontal,direction,up);
	horizontal.Normalize();
	Vec3f::Cross3(screen_up,horizontal,direction);
}

void Camera::glInit(int w, int h)
{ 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(angle*180.0/3.14159, (float)w/float(h), 0.5, 40.0); 
} 

void Camera::glPlaceCamera(void)
 {
 	 gluLookAt(center.x(), center.y(), center.z(), center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(), up.x(), up.y(), up.z()); 
} 

void Camera::dollyCamera(float dist) { center += direction*dist;}

void Camera::truckCamera(float dx, float dy) { center += horizontal*dx + screen_up*dy;}

void Camera::rotateCamera(float rx, float ry)
 { 
	float tiltAngle = acos(up.Dot3(direction)); 
	if (tiltAngle-ry > 3.13) ry = tiltAngle - 3.13; 
	else if (tiltAngle-ry < 0.01) ry = tiltAngle - 0.01; 
	Matrix rotMat = Matrix::MakeAxisRotation(up, rx); 
	rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
	rotMat.Transform(center); 
	rotMat.TransformDirection(direction); 
	rotMat.TransformDirection(up);
	direction.Normalize();
	Vec3f::Cross3(horizontal,direction,up);
	horizontal.Normalize();
	Vec3f::Cross3(screen_up,horizontal,direction);
}
