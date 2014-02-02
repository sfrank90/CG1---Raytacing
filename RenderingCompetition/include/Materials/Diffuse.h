#ifndef __DIFFUSE_H__
#define __DIFFUSE_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/
#include "../SpectralDistribution.h"
#include "../Material.h"
#include "../Random.h"

class Diffuse : public Material {
protected:
	Distribution* mDistribution;
public:
	Diffuse(Distribution* d) : mDistribution(d) {}
	// Importance sampling
	virtual glm::vec3 sample(const Ray &ray, float wavelength) const {
		if(!ray.hasHit)
			return glm::vec3(0.f);

		glm::vec3 normal = ray.hitNormal;

		/* Align the normal with the incident vector. */
		if(glm::dot(ray.dir, normal) > 0)
			normal = -normal;

		/* Move the origin outside the surface slightly. */

		RandomFloat rnd;

		/* Get two random numbers. */
		float u1 = rnd();
		float u2 = rnd();

		/* Compute a cosine-weighted vector. */
		const float theta = acos(sqrt(1.0f - u1));
		const float phi = constants::two_pi * u2;

		/* Get the unit vector. */
		glm::vec3 dir(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));

		glm::vec3 up = dir;
		if(fabs(up.x) <= fabs(up.y) && fabs(up.x) <= fabs(up.z)) {
			up.x = 1.0f;
		} else if(fabs(up.y) <= fabs(up.x) && fabs(up.y) <= fabs(up.z)) {
			up.y = 1.0f;
		} else {
			up.z = 1.0f;
		}

        glm::vec3 x = glm::normalize(glm::cross(up, normal));
        glm::vec3 z = glm::normalize(glm::cross(x, normal));

        /* Transform the unit vector to this basis. */
        return glm::normalize(x * dir.x + normal * dir.y + z * dir.z);
	}
	virtual float brdf(const Ray &ray, const glm::vec3 &exitant, float wavelength, bool sampled) const {
	  /* If the exitant ray was importance-sampled... */
		if (sampled) {
			/* This is just a constant by definition, but we are using a cosine-weighted distribution so we divide
			 * the reflectance by the cosine-weighted PDF (which happens to be cos(exitant, normal)). Note the
			 * ubiquitous 1/pi factor happens to cancel out cleanly due to the sampling scheme. */
			return std::max(mDistribution->intensity(wavelength), 0.0f);
		} else {
			/* Otherwise, use the uniform sampling formulation. */
			return std::max(2.0f * mDistribution->intensity(wavelength) * std::abs(glm::dot(exitant, ray.hitNormal)), 0.0f);
		}
	}
};

#endif