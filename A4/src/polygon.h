#ifndef _POLYGON_H_
#define _POLYGON_H_

//#define finf 1.0*1e10
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <vector>
#include "matrix.h"
#include "material.h"
#include "edge.h"
#define GL_SILENCE_DEPRECATION
using namespace std;
class polygon{
	public:
	//constructors
	  polygon(vector<Vec3f>& _vert,int _id,Material* m):vertNum(_vert.size()),edgeNum(_vert.size()),id(_id),mtr(m){
	  	vert=new Vec3f[vertNum];
	  	edge=new Edge3f[vertNum];
	  	for(int i=0;i<vertNum;i++) vert[i]=_vert[i];
	  	for(int i=0;i<edgeNum;i++) edge[i].Set(vert[i],vert[(i+1)%edgeNum]);
	  }
	//destructor
	  ~polygon(){
	  	if(vert!=NULL){
	  		delete[] vert;
	  		vert=NULL;
	  	}
	  if(edge!=NULL){
	  		delete[] edge;
	  		edge=NULL;
	  	}	  
	  }
	  //Transformations
	  //3D
	 void applyTransform(Matrix& m){
	 	for(int i=0;i<vertNum;i++)
	 		m.Transform(vert[i]);
	 	for(int i=0;i<edgeNum;i++)
	 		edge[i].applyTransform(m);
	 }
	 void calEquation(){
	 	Vec3f a=vert[0];
		Vec3f b=vert[1];
		Vec3f c=vert[2];
		Vec3f ab=b-a;
		Vec3f ac=c-a;
		Vec3f::Cross3(norm,ab,ac);
		norm.Normalize();
		dConst=norm.Dot3(a);
	 }
	 void paint(){
	 	mtr->glSetMaterial();
	 	glBegin(GL_TRIANGLES);
	 	glNormal3f(norm.x(),norm.y(),norm.z());
         for(int i=0;i<vertNum;i++){
	 		glVertex3f(vert[i].x(),vert[i].y(),vert[i].z());
         }
	 	glEnd();
	 }
 private:
		int vertNum;
		int edgeNum;
		Vec3f* vert;
		Edge3f* edge;
		Vec3f norm;
		float dConst;
		int id;
		Material* mtr;
};



#endif
