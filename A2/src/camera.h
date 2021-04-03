#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "matrix.h"
class Camera{
	public:
		Camera(Vec3f&,Vec3f&,Vec3f&,float);
		void glInit(int w, int h) ; 
		void glPlaceCamera(void); 
		void dollyCamera(float dist); 
		void truckCamera(float dx, float dy); 
		void rotateCamera(float rx, float ry); 
	private:
		Vec3f center;
		Vec3f direction;
		Vec3f up;
		Vec3f screen_up;
		Vec3f horizontal;
		float angle;
};
#endif
