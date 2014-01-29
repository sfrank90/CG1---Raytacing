/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*
* TODO: Hit object and hit material...
*
*/

#include <glm/glm.hpp>
#include "../include/Common.h"
#include "../include/Camera.h"


void PerspectiveCamera::init() {
	// Local coordinate system (ortho normal).
	zAxis = glm::normalize(mLookAt - mEye);
	xAxis = glm::normalize(glm::cross(zAxis, mUp));
	yAxis = glm::normalize(glm::cross(xAxis, zAxis));
	
	float angleInRad = (mFOV * constants::pi) / 180.f;
	//mFocus = 1.0 / tan(angleInRad / 2.);
	mFocus = 0.5f / tan(angleInRad/2.);
	zAxis *= mFocus;
}

bool PerspectiveCamera::initRay(float x, float y, Ray &ray) const {
	ray.reset();
	ray.origin = mEye;
	ray.dir = x * xAxis + y * yAxis +  zAxis; 
	ray.dir = glm::normalize(ray.dir);
	return true;
}
  bool PerspectiveCamera::initRay(float x, float y, float offX, float offY, Ray &ray) const { 
	initRay(x, y, ray);
	ray.origin = ray.origin + glm::vec3(offX*mApertureRadius, offY*mApertureRadius, 0.f);
	ray.dir = ray.dir * mFocalPlane; 
	ray.dir = ray.dir - glm::vec3(offX*mApertureRadius, offY*mApertureRadius, 0.f);
	ray.dir = glm::normalize(ray.dir);

	return true;
  }
