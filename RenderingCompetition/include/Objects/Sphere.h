#ifndef __SPHERE_H__
#define __SPHERE_H__

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

class Sphere : public Object {
public:
	Sphere(const glm::vec3 &center, float radius, std::shared_ptr<Material> m) : Object(m), mCenter(center), mRadius(radius)
	{}

	bool intersect(Ray &ray) {
		float phi;
		glm::vec3 phit;
		glm::vec3 new_origin = ray.origin-mCenter;

		// Compute quadratic sphere coefficients
		float A = glm::dot(ray.dir,ray.dir);//ray.dir.x*ray.dir.x + ray.dir.y*ray.dir.y + ray.dir.z*ray.dir.z;
		float B = 2 * glm::dot(ray.dir, new_origin);//(ray.dir.x*ray.origin.x + ray.dir.y*ray.origin.y + ray.dir.z*ray.origin.z);
		float C = glm::dot(new_origin,new_origin) - mRadius*mRadius;

		// Use 'abc'-formula for finding root t_1,2 = (-b +/- sqrt(b^2-4ac))/(2a)
		float discriminant = B * B - 4 * A * C;
		if (discriminant < 0)
		  return false;
   
		float root = sqrt(discriminant);
		float dist = (-B - root) / (2 * A); // try t0.
		if (dist > ray.t)
		  return false;

		// Test if t is closer than the current ray.t and if t is not zero.
		// If so, set the new ray.t and return true
		if (dist < constants::epsilon) { // i.e. t0 behind camera.
		  dist = (-B + root) / (2 * A); // try t1.
		  if (dist < constants::epsilon || dist > ray.t)
			return false;
		}
		ray.t = dist;
		ray.hitObject = std::shared_ptr<Object>(this);
		ray.hitNormal = glm::normalize(ray.origin + ray.t * ray.dir - mCenter);
		ray.hasHit = true;
		
		return true;
	}

	AABB computeAABB() const {
		AABB bb;
		bb.addPoint(mCenter + glm::vec3(mRadius));
		bb.addPoint(mCenter - glm::vec3(mRadius));
		return bb;	
	}

private:
	glm::vec3 mCenter;
	float     mRadius;
};

#endif