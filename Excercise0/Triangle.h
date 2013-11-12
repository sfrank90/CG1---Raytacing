#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Vec3f.h"

namespace cg1 {

class Triangle {
public:
	Vec3f a,b,c;
	Triangle(Vec3f a, Vec3f b, Vec3f c) : a(a),b(b),c(c) {};
	~Triangle() {}

	inline Vec3f getNormal() {
		return ((b-a) % (c-a)).normalized();
	}
};

}
#endif