#ifndef __CAMERA_H__
#define __CAMERA_H__

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

#include <glm/glm.hpp>
#include "Ray.h"

class Camera {
protected:
	int       mWidth, mHeight;       
	float     mAspect, mNear, mFar;         
	glm::vec3 mEye, mLookAt, mUp;  

public: 
	Camera(int resx, 
			int resy, 
			glm::vec3 eye, 
			glm::vec3 lookat, 
			glm::vec3 up)  
			: mWidth(resx), mHeight(resy), mAspect(0.f), mEye(eye), mLookAt(lookat), mUp(up) {
		// Aspect ratio.
		mAspect = float(resx) / float(resy);
		mNear = 1.0e-3f;
		mFar  = 1.0e5f;
	}

	virtual ~Camera() {}

	/**
	* Returns a ray going through the given pixel coordinate.
	*/
	virtual bool initRay(float x, float y, Ray &ray) const = 0;

	/**
	* Returns a ray going through the given pixel coordinate.
	*/
	virtual bool initRay(float x, float y, float offX, float offY, Ray &ray) const = 0;
};

class PerspectiveCamera: public Camera {
protected:
	float     mFOV;                       // Vertical opening angle of the camera.
	glm::vec3 xAxis, yAxis, zAxis;    // Local coordinate system.
	float     mFocus;                      // Reciprocal of vertical opening angle.
  
	//Camera parameter
	float mApertureRadius;
	float mFocalPlane;

public:
	PerspectiveCamera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up, float fov, int resX, int resY, float aperture = 0.5f, float focal = 5.f)
		: Camera(resX, resY, eye, lookat, up), mFOV(fov), mApertureRadius(aperture), mFocalPlane(focal) {
		init();
	}

	virtual ~PerspectiveCamera()
	{}

	void init();

	void setApertureRadius(float a) {
		mApertureRadius = a;
	}
	void setFocalPlane(float f) {
		mFocalPlane = f;
	}

	virtual bool initRay(float x, float y, Ray &ray) const;
	virtual bool initRay(float x, float y, float offX, float offY, Ray &ray) const;

};

#endif