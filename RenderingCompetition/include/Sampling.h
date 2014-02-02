#ifndef __SAMPLING_H__
#define __SAMPLING_H__   

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/


#include "Random.h"
#include "Common.h"
#include <glm/glm.hpp>

class Sampling {
private:
	RandomFloat rnd;
public:
	Sampling() {}
	~Sampling() {}
	glm::vec3 sampleHemisphere() {
		glm::vec3 result;

		double u1 = (double)rnd();
		double u2 = (double)rnd();

		result.x = cos(2*constants::pi*u2)*2*sqrt(1.0 - u1*u1);
		result.y = sin(2*constants::pi*u2)*2*sqrt(1.0 - u1*u1);
		result.z = u1; 

		return result;
	}

	glm::vec3 sampleSphere() {
		glm::vec3 result;

		double u1 = (double)rnd();
		double u2 = (double)rnd();

		result.x = cos(2*constants::pi*u2)*2*sqrt(u1*(1.0 - u1));
		result.y = sin(2*constants::pi*u2)*2*sqrt(u1*(1.0 - u1));
		result.z = 1.0 - 2.0*u1;

		return result;
	}

	glm::vec3 sampleHemisphereCW() {
		glm::vec3 result;
		double u1 = (double)rnd();
		double u2 = (double)rnd();

		double th = 2*constants::pi*u2;
		double r = sqrt(u1);
		result.x = cos(th)*r;
		result.y = sin(th)*r;
		result.z = 1.0 - result.x*result.x - result.y*result.y;
		if (result.z <= 0.0) 
			result.z = 0.0;
		else 
			result.z = sqrt(result.z);
		return result;
	}

	void rotate_to_normal(const glm::vec3& normal, glm::vec3& v) {
		if(normal[2] < -0.9999999f) {
			v = glm::vec3(-v[1], -v[0], -v[2]);
			return;
		}
		const float a = 1.0f/(1.0f + normal[2]);
		const float b = -normal[0]*normal[1]*a;
		v =   glm::vec3(1.0f - normal[0]*normal[0]*a, b, -normal[0])*v[0] 
			+ glm::vec3(b, 1.0f - normal[1]*normal[1]*a, -normal[1])*v[1] 
			+ normal*v[2];
	}
	glm::vec3 sampleCW(const glm::vec3& normal) {
		// ref: http://www.rorydriscoll.com/2009/01/07/better-sampling/
		// ref: http://pathtracing.wordpress.com/2011/03/03/cosine-weighted-hemisphere/
		// Get random numbers
		const float r1 = rnd();
		const float r2 = rnd();

		const float theta = acos(sqrt(1.0f - r1));
		const float phi = 2.0f * constants::pi * r2;

			// Calculate new direction as if the z-axis were the normal
		glm::vec3 _normal(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

		// Rotate from z-axis to actual normal and return
		rotate_to_normal(normal, _normal);
		return _normal;
	}

};

#endif