#ifndef __ISOTROPICLIGHT_H__
#define __ISOTROPICLIGHT_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/

#include "../Light.h"
#include "../SpectralDistribution.h"
class IsotropicLight : public Light {
private:
	std::shared_ptr<Distribution> mDistribution;
public:
	IsotropicLight(std::shared_ptr<Distribution> d) :  mDistribution(d) {}
	
	float emit(Ray &ray, float wavelength) const {
		return mDistribution->intensity(wavelength);
	}
};



#endif