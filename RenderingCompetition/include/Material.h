#ifndef __MATERIAL_H__
#define __MATERIAL_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/
#include "Ray.h"

class Material {
public:
	//Wavelength dependend!
	float mExtinctionInside, mExtinctionOutside; //for imaginary ior
	virtual glm::vec3 sample(const Ray &ray, float wavelength) {
		return glm::vec3(0.0f);
	}
	virtual float brdf(const Ray &ray, const glm::vec3 &exitant, float wavelength, bool sampled) {
		return 0.f;
	}


};

#endif