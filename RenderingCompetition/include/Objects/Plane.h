#ifndef __PLANE_H__
#define __PLANE_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*
*/

#include "../Common.h"
#include "Triangle.h"

class InfinitePlane : public Object {
public:
	InfinitePlane(glm::vec3 normal, glm::vec3 origin, Material *m, Light *l = nullptr) : Object(m,l), mNormal(normal), mOrigin(origin) {}

	bool intersect(Ray &ray) {
		glm::vec3 diff = mOrigin - ray.origin;
		float t = glm::dot(diff, mNormal) / glm::dot(ray.dir, mNormal);
		if (t < constants::epsilon || t > ray.t) {
			return false;
		}

		ray.t = t;
		ray.hitObject = this;
		ray.hasHit = true;
		ray.hitNormal = mNormal;
		return true;
	}

	AABB computeAABB() const {
		AABB bb;
		if (mNormal.x == 0 && mNormal.y == 0) {
			bb.addPoint(glm::vec3(-constants::infinity, -constants::infinity, mOrigin.z));
			bb.addPoint(glm::vec3(constants::infinity, constants::infinity, mOrigin.z));
		} else if (mNormal.x == 0 && mNormal.z == 0) {
			bb.addPoint(glm::vec3(-constants::infinity, mOrigin.y, -constants::infinity));
			bb.addPoint(glm::vec3(constants::infinity, mOrigin.y, constants::infinity));
		} else if (mNormal.y == 0 && mNormal.z == 0) {
			bb.addPoint(glm::vec3(mOrigin.x,-constants::infinity, -constants::infinity));
			bb.addPoint(glm::vec3(mOrigin.x,constants::infinity, constants::infinity));
		}
		return bb;
	}
private:
	glm::vec3 mNormal;
    glm::vec3 mOrigin;
};

class Plane : public Object {
public:
	Plane(glm::vec3 normal, glm::vec3 a, glm::vec3 b,glm::vec3 c,glm::vec3 d, Material *m, bool l=false) : Object(m,l), mNormal(normal), mA(a), mB(b), mC(c), mD(d) {}

	bool canIntersect() {
		false;
	}

	void refine(std::vector<Object*> &refined) const {
		//glm::vec3 up = mNormal;
		//if(fabs(up.x) <= fabs(up.y) && fabs(up.x) <= fabs(up.z)) {
		//	up.x = 1.0f;
		//} else if(fabs(up.y) <= fabs(up.x) && fabs(up.y) <= fabs(up.z)) {
		//	up.y = 1.0f;
		//} else {
		//	up.z = 1.0f;
		//}

		//glm::vec3 x = glm::normalize(glm::cross(up,mNormal)); 
		//glm::vec3 y = glm::normalize(glm::cross(x, mNormal)); 

		//glm::vec3 a = mOrigin + x*mExtendX + y*mExtendY,
		//		  b = mOrigin + x*mExtendX - y*mExtendY,
		//		  c = mOrigin - x*mExtendX + y*mExtendY,
		//		  d = mOrigin - x*mExtendX - y*mExtendY;


		Object *t1 = new Triangle(mA,mB,mC, mNormal,mNormal,mNormal, mMaterial),
			   *t2 = new Triangle(mC,mD,mA, mNormal,mNormal,mNormal, mMaterial);

		refined.push_back(t1);
		refined.push_back(t2);
	}

	AABB computeAABB() const {
		AABB bb;
		//glm::vec3 up = mNormal;
		//if(fabs(up.x) <= fabs(up.y) && fabs(up.x) <= fabs(up.z)) {
		//	up.x = 1.0f;
		//} else if(fabs(up.y) <= fabs(up.x) && fabs(up.y) <= fabs(up.z)) {
		//	up.y = 1.0f;
		//} else {
		//	up.z = 1.0f;
		//}

		//glm::vec3 x = glm::normalize(glm::cross(up,mNormal)); 
		//glm::vec3 y = glm::normalize(glm::cross(x, mNormal)); 

		//glm::vec3 a = mOrigin + x*mExtendX + y*mExtendY,
		//		  b = mOrigin + x*mExtendX - y*mExtendY,
		//		  c = mOrigin - x*mExtendX + y*mExtendY,
		//		  d = mOrigin - x*mExtendX - y*mExtendY;

		bb.addPoint(mA);
		bb.addPoint(mB);
		bb.addPoint(mC);
		bb.addPoint(mD);
		return bb;
	}
private:
	glm::vec3 mNormal;
    glm::vec3 mA, mB, mC, mD;
	float mExtendX, mExtendY;
};


#endif