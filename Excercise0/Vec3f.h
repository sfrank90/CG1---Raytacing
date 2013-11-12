#ifndef __VEC3F_H__
#define __VEC3F_H__

#include <cmath>
#include <fstream>

namespace cg1 {

class Vec3f {
public:
	union {
		//Either use simple coordinates
        struct { 
            float x;    
            float y;
            float z;
        };
		//or use an array
        float c[3];       
    };

	//constructors
	inline Vec3f() : x(0), y(0), z(0) {}    
	inline Vec3f(const float f) : x(f), y(f), z(f) {}    
    inline Vec3f(const float x, const float y, const float z) : x(x), y(y), z(z) {}    
	inline Vec3f(const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2])  {}    
	inline Vec3f(const Vec3f &v) : x(v.x),y(v.y),z(v.z) {}
	//destrcutor
	~Vec3f() {}

	//assignment
	inline Vec3f& operator=(const Vec3f &v) {
		if(this != &v)
			x = v.x; y = v.y; z = v.z; 
		return *this;
	}

	//addition 
	inline Vec3f operator+(const Vec3f &v) const {
		return Vec3f(x+v.x, y+v.y, z+v.z);
	}
	inline Vec3f& operator+=(const Vec3f &v) {
		*this = *this + v;
		return *this;
	}

	//subtraction
	inline Vec3f operator-(const Vec3f &v) const {
		return Vec3f(x-v.x, y-v.y, z-v.z);
	}
	inline Vec3f& operator-=(const Vec3f &v)  {
		*this = *this - v;
		return *this;
	}

	//dot-product
	inline float operator*(const Vec3f& v) const {
		return x*v.x + y*v.y + z*v.z;
	}
	inline float dot(const Vec3f& v) const {
		return *this * v;
	}

	//length
	inline float length() const {
		return sqrt((*this) * (*this));
	}

	//cross-product copy
	inline Vec3f operator%(const Vec3f& v) const {
        return Vec3f(
            y*v.z - v.y*z,
            z*v.x - v.z*x,
            x*v.y - v.x*y
        );
	}
	inline Vec3f& operator%=(const Vec3f& v)  {
		*this = *this % v;
		return *this;
	}

	//cross-product copy
	inline Vec3f cross(const Vec3f& v) const {
		Vec3f tmp(*this);
		tmp %= v;
		return tmp;
	}

	inline void normalize() {
		if(length() == 0.0f) 
			*this = Vec3f(0.0f,0.0f,0.0f);
		else
			*this = *this * (float)(1.0f/length());
	}

	inline Vec3f normalized() const {
		Vec3f v = *this;
		if(v.length() == 0.0f) 
			return Vec3f(0.0f,0.0f,0.0f);
		else
			return v * (float)(1.0f/v.length());
	}

	inline Vec3f operator*(float s) const {
		return Vec3f(x*s, y*s, z*s);
	}
	inline Vec3f& operator*=(float s)  {
		*this = *this * s;
		return *this;
	}
};

}
#endif