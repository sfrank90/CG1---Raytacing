#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3f.h"
#include "Ray.h"

class Camera
{
public:
  int resX, resY;       // Image resolution in pixels.
  float aspect;         // Aspect ratio X:Y.
  Vec3f pos, dir, up;   // Position to be at, direction to look, where the head is.

  Camera(int resX, int resY, // camera resolution
      Vec3f pos, Vec3f dir, Vec3f up) // position and orientation
      : resX(resX), resY(resY), aspect(0.f), pos(pos), dir(dir), up(up)
  {
    // Aspect ratio.
    aspect = float(resX) / float(resY);
  }

  virtual ~Camera()
  {
  }

  /**
   * Returns a ray going through the given pixel coordinate.
   */
  virtual bool initRay(float x, float y, Ray &ray) = 0;

  /**
   * Returns a ray going through the given pixel coordinate,
   * and shifted by given offset.
   */
  virtual bool initRay(float x, float y, float offX, float offY, Ray &ray) = 0;

};

#endif
