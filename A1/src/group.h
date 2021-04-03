#ifndef _GROUP_H_
#define _GROUP_H_

#include "matrix.h"
#include "polygon.h"
#include <vector>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

 
using namespace std;

class polygon;
class Group{
public:
	Group(){
		Trans.SetToIdentity();
	}
	~Group(){
		for(unsigned int i=0;i<polylist.size();i++)
		if(polylist[i]!=NULL){
			delete polylist[i];
			polylist[i]=NULL;
		}
	}
	void addNewPoly(polygon* poly){
		polylist.push_back(poly);
	}
	void leftMulMatrix(Matrix& m){
		Trans=m*Trans;
	}
	void applyTransform(){
		for(unsigned int i=0;i<polylist.size();i++)
			polylist[i]->applyTransform(Trans);
	}
    void applyTransform(Matrix m){
        for(unsigned int i=0;i<polylist.size();i++)
            polylist[i]->applyTransform(m);
    }
    void calEquation()
    {
    	 for(unsigned int i=0;i<polylist.size();i++)
            polylist[i]->calEquation();
    }
	unsigned long int getPolyNum(){
		return polylist.size();
	}
	polygon* getPoly(unsigned long int i){
		return polylist[i];
	}
	void paint(){
		for(unsigned int i=0;i<polylist.size();i++) polylist[i]->paint();
	}
   
	private:
	vector<polygon*> polylist;
	Matrix Trans;
};

#endif
