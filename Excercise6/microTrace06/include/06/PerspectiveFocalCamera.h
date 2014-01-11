#pragma once
#ifndef PERSPECTIVEFOCALCAMERA_H
#define PERSPECTIVEFOCALCAMERA_H

#include "../01/PerspectiveCamera.h"

/**
 * Camera with aperture size and focal plane distance. Allows depth-of-field.
 */
class PerspectiveFocalCamera: public PerspectiveCamera
{
  float aperture;   // Size of lens aperture.
  float focal;      // Focal plane.

public:

  PerspectiveFocalCamera(Vec3f pos, Vec3f dir, Vec3f up, float angle, // standard params
      float aperture, float focal, // aperture size and focal length
      int resX, int resY) // resolution
      : PerspectiveCamera(pos, dir, up, angle, resX, resY), aperture(aperture), focal(focal)
  {
  }

  virtual ~PerspectiveFocalCamera()
  {
  }

  virtual bool initRay(float x, float y, float offX, float offY, Ray &ray)
  {
    PerspectiveCamera::initRay(x, y, ray);

    // IMPLEMENT ME 6.2
    // Vary base ray with given pixel offset.
    // IMPLEMENT ME END.

	//quadratic oder circular?
	ray.org = ray.org + Vec3f(offX*aperture, offY*aperture, 0.f);
	ray.dir = ray.dir * focal; //scale along direction
	// 
	ray.dir = ray.dir - Vec3f(offX*aperture, offY*aperture, 0.f);
    normalize(ray.dir);
    return true;
  }

};

#endif
