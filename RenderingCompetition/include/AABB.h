#ifndef __AABB_H__
#define __AABB_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/

#include <glm/glm.hpp>
#include <functional> //for swap
#include "Common.h"
#include "Ray.h"

struct AABB {
	glm::vec3 p_min, p_max;

	AABB() : p_min(constants::infinity), p_max(-constants::infinity) { }
	AABB(const glm::vec3& min_point, const glm::vec3& max_point) : p_min(min_point), p_max(max_point) { }

	void reset() {
		p_max = glm::vec3(-constants::infinity);
		p_min = glm::vec3(constants::infinity);
	}

	void addPoint(const glm::vec3& p) {
		p_min = glm::min(p_min, p);
		p_max = glm::max(p_max, p);
	}

	void addPoint(float x, float y, float z) {
		addPoint(glm::vec3(x, y, z));
	}

	void addAABB(const AABB& other) {
		p_min = glm::min(p_min, other.p_min);
		p_max = glm::max(p_max, other.p_max);
	}

	bool intersect(const AABB& other) const {
		for(std::uint8_t i = 0; i < 3; ++i)
			if(other.p_min[i] - constants::epsilon> p_max[i] || other.p_max[i] +constants::epsilon < p_min[i])
			return false;
		return true;
	}

	bool intersect(Ray& r) const {
		float tmin = (p_min.x - r.origin.x) / r.dir.x;
		float tmax = (p_max.x - r.origin.x) / r.dir.x;
		if (tmin > tmax) std::swap(tmin, tmax);
		float tymin = (p_min.y - r.origin.y) / r.dir.y;
		float tymax = (p_max.y - r.origin.y) / r.dir.y;
		if (tymin > tymax) std::swap(tymin, tymax);
		if ((tmin > tymax) || (tymin > tmax))
			return false;
		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;
		float tzmin = (p_min.z - r.origin.z) / r.dir.z;
		float tzmax = (p_max.z - r.origin.z) / r.dir.z;
		if (tzmin > tzmax) std::swap(tzmin, tzmax);
		if ((tmin > tzmax) || (tzmin > tmax))
			return false;
		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;
		if ((tmin > r.tmax) || (tmax < r.tmin)) return false;
		if (r.tmin < tmin) r.tmin = tmin;
		if (r.tmax > tmax) r.tmax = tmax;

		r.t = tmin;
		r.hasHit = true;
		return true;
	}

	void clipRay(Ray &ray,float &tnear,float &tfar) {
		float d, den;

		den = 1./ray.dir.x;
		if (ray.dir.x > 0) 
		{
			if ((d = (p_min.x-ray.origin.x)*den) > tnear) tnear = d;
			if ((d = (p_max.x-ray.origin.x)*den) < tfar) tfar = d;
		} 
		else  
		{
			if ((d = (p_max.x-ray.origin.x)*den) > tnear) tnear = d;
			if ((d = (p_min.x-ray.origin.x)*den) < tfar) tfar = d;
		}
		if (tnear > tfar) return;

		den = 1./ray.dir.y;
		if (ray.dir.y > 0) 
		{
			if ((d = (p_min.y-ray.origin.y)*den) > tnear) tnear = d;
			if ((d = (p_max.y-ray.origin.y)*den) < tfar) tfar = d;
		} 
		else 
		{
			if ((d = (p_max.y-ray.origin.y)*den) > tnear) tnear = d;
			if ((d = (p_min.y-ray.origin.y)*den) < tfar) tfar = d;
		} 
		if (tnear > tfar) return;

		den = 1./ray.dir.z;
		if (ray.dir.z > 0) 
		{
			if ((d = (p_min.z-ray.origin.z)*den) > tnear) tnear = d;
			if ((d = (p_max.z-ray.origin.z)*den) < tfar) tfar = d;
		} 
		else 
		{
			if ((d = (p_max.z-ray.origin.z)*den) > tnear) tnear = d;
			if ((d = (p_min.z-ray.origin.z)*den) < tfar) tfar = d;
		} 
		return;
		}

		float area() const  {
		const glm::vec3 d = diagonal();
		return 2.0f*(d[0]*d[1] + d[1]*d[2] + d[2]*d[0]);
	}

	glm::vec3 center() const { 
		return (p_max + p_min)*0.5f; 
	}
	glm::vec3 diagonal() const { 
		return p_max - p_min; 
	}
	glm::vec3 lower_left_far() const { 
		return p_min; 
	}
	glm::vec3 lower_left_near() const { 
		return glm::vec3(p_min[0], p_min[1], p_max[2]); 
	}
	glm::vec3 lower_right_far() const { 
		return glm::vec3(p_max[0], p_min[1], p_min[2]); 
	}
	glm::vec3 lower_right_near() const { 
		return glm::vec3(p_max[0], p_min[1], p_max[2]); 
	}
	glm::vec3 upper_left_far() const { 
		return glm::vec3(p_min[0], p_max[1], p_min[2]); 
	}
	glm::vec3 upper_left_near() const { 
		return glm::vec3(p_min[0], p_max[1], p_max[2]); 
	}
	glm::vec3 upper_right_far() const { 
		return glm::vec3(p_max[0], p_max[1], p_min[2]); 
	}
	glm::vec3 upper_right_near() const { 
		return p_max; 
	}
};

#endif