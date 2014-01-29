#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

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
#include "../Object.h"

class Triangle : public Object {
public:
	Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 NA, glm::vec3 NB, glm::vec3 NC, std::shared_ptr<Material> m) 
			: Object(m), mA(A), mB(B), mC(C), mNA(NA), mNB(NB), mNC(NC)  {}

	virtual bool intersect(Ray &ray) {
		const glm::vec3 edge1 = mB - mA;
		const glm::vec3 edge2 = mC - mA;

		const glm::vec3 pvec = glm::cross(ray.dir, edge2);

		const float det = glm::dot(edge1, pvec);
		if (fabs(det) < constants::epsilon)
			return false;


		const float inv_det = 1.0f / det;

		const glm::vec3 tvec = ray.origin - mA;
		float lambda = glm::dot(tvec, pvec);
		lambda *= inv_det;

		if (lambda < 0.0f || lambda > 1.0f)
			return false;

		const glm::vec3 qvec = glm::cross(tvec,edge1);
		float mue = glm::dot(ray.dir, qvec);
		mue *= inv_det;

		if (mue < 0.0f || mue + lambda > 1.0f)
			return false;


		float f = glm::dot(edge2, qvec);
		f *= inv_det;
		if (ray.t <= f || f < constants::epsilon)
			return false;

		ray.t = f;
		ray.hitObject = std::shared_ptr<Object>(this);
		ray.hasHit = true;
		ray.u = lambda;
		ray.v = mue;

		//Calc. normal
		glm::vec3 normal = lambda * mNB + mue * mNC + (1 - lambda - mue) * mNA;
		ray.hitNormal = glm::normalize(normal); //smooth normals

		return true;
	}

	AABB computeAABB() const {
		AABB bb;
		bb.addPoint(mA);
		bb.addPoint(mB);
		bb.addPoint(mC);
		return bb;		  
	}

private:
	glm::vec3 mA, mB, mC;
	glm::vec3 mNA, mNB, mNC; // vertex normals
};

#endif