#ifndef _EDGE_H_
#define _EDGE_H_

#include "matrix.h"

class Edge3f{
	public:
		Edge3f(){}
		Edge3f(Vec3f& _begin, Vec3f& _end):bbegin(_begin),eend(_end){}
		Vec3f begin(){
			return bbegin;
		}
		Vec3f end(){
			return eend;
		}
		void applyTransform(Matrix& m){
			m.Transform(bbegin);
			m.Transform(eend);
		}
		void Set(Vec3f& _begin, Vec3f& _end){
			bbegin=_begin;
			eend=_end;
		}
	private:
		Vec3f bbegin;
		Vec3f eend;
};
#endif
