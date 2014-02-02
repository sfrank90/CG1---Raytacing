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
	glm::vec3 mAmbient, mDiffuse, mSpecular, mTransmission;
	float mScatterCoeff, mAbsorptionCoeff;
	float shininess, ior;

	bool mParticitatingMedium;

	virtual glm::vec3 sample(const Ray &ray, float wavelength) const {
		return glm::vec3(0.0f);
	}
	virtual float brdf(const Ray &ray, const glm::vec3 &exitant, float wavelength, bool sampled) const {
		return 0.f;
	}

	Material() {
		shininess = 0.f;
		ior = 1.0003f;
		mParticitatingMedium = false;
	}
};

#endif