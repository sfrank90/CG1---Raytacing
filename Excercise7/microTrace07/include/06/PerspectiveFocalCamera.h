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
    
    // offX, offY are in [-0.5 .. 0.5] (see render-call in Renderer06.h)

    // Vary base ray with given pixel offset.
    // assuming d(imagePlane, lens) == d(lens, focalPlane) == focal
    // -> d(ray.org, focalPlane) == 2 * focal
    Vec3f offset = (offX * 4.f * aperture) * xAxis + (offY * 4.f * aperture) * yAxis;
    ray.org = ray.org + offset;
    ray.dir = (2.f * focal * ray.dir) - offset;
    normalize(ray.dir);

    return true;
  }

};

#endif
