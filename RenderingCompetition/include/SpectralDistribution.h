#ifndef __SPECTRALDISTRIBUTION_H__
#define __SPECTRALDISTRIBUTION_H__

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

class Distribution {
public:
	//the spectral radiance...
    virtual float intensity(float wavelength) = 0;
};

//important for lights because they are actually black bodies (meaning they absorb almost any incoming radiance)...
class BlackBodyDistribution : public Distribution { 
private:
	float mTemperature; // [K] (kelvin)
public:
	BlackBodyDistribution(float temperature) : mTemperature(temperature) {}
	float intensity(float wavelength) {
		wavelength *= 1e-9f; //to meters

		// Plank's Law:
		//
		// B_lambda = (2*h*c^2 / lamda^5) * (1 / ( exp(h*c/(lambda*k_B*T)) - 1))
		//
		// h: plank's constant 
		// k_B: Boltzmann-constant
		// c: speed of light
		// lambda: wavelength
		// T: temperture
		float powerTerm  = 3.74183e-16f * glm::pow(wavelength, -5.0f);
		return powerTerm / (glm::exp(1.4388e-2f / (wavelength * mTemperature)) - 1.0f);
	}
};

//important for lights because they are actually black bodies (meaning they absorb almost any incoming radiance)...
class ConstantDistribution : public Distribution { 
private:
	float mConstant; 
public:
	ConstantDistribution(float wavelength) : mConstant(wavelength) {}
	float intensity(float wavelength) {
		return mConstant;
	}
};



class MonochromaticDistribution : public Distribution { 
private:
	float mPeak; 
public:
	MonochromaticDistribution(float wavelength) : mPeak(wavelength) {}
	float intensity(float wavelength) {
		return glm::exp(-glm::pow(wavelength - mPeak, 2.0f) * 0.002f);
	}
};

class SellmeierDistribution : public Distribution { 
private:
	glm::vec3 mB,mC;
public:
	SellmeierDistribution(glm::vec3 B, glm::vec3 C) : mB(B), mC(C) {}
	float intensity(float wavelength) {
		wavelength *= 1e-3f; //to micrometer

		//Refractive index by sellmeier formular
		float index = 1.0f;
		for (int t = 0; t < 3; ++t) 
			index += (mB[t] * pow(wavelength, 2.0f)) / (pow(wavelength, 2.0f) - mC[t]);
		return sqrt(index);
	}
};
#endif