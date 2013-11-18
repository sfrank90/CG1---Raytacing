#pragma once
#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "../Camera.h"

class PerspectiveCamera: public Camera
{

protected:
  float angle;                  // Vertical opening angle of the camera.
  Vec3f xAxis, yAxis, zAxis;    // Local coordinate system.
  float focus;                  // Reciprocal of vertical opening angle.

public:

  PerspectiveCamera(Vec3f pos, Vec3f dir, Vec3f up, float angle, int resX, int resY)
      : Camera(resX, resY, pos, dir, up), angle(angle)
  {
    // Use your mathematical derivation from task 1.1.
    //
    // (1) Calculate zAxis, yAxis and xAxis.
    // (2) Normalize them.
    // (3) Calculate the aspect ratio (or use from super constructor).
    // (4) Calculate the opening angle in radians.
    // (5) Calculate the focus (distance to plane in pixels).

    // Local coordinate system (ortho normal).
    zAxis = dir;
    xAxis = cross(dir, up);
    yAxis = cross(xAxis, zAxis);
    normalize(xAxis);
    normalize(yAxis);
    normalize(zAxis);

    // Opening angle:
    float angleInRad = angle * M_PI / 180.;
    focus = 1.0 / tan(angleInRad / 2.);

  }

  virtual ~PerspectiveCamera()
  {
  }

  virtual bool initRay(float x, float y, Ray &ray)
  {
    // Use your mathematical derivation from task 1.1.
    //
    // (1) Calculate the origin of the ray.
    // (2) Calculate the direction of the ray.
    // (3) Set the current distance of the nearest object that intersects
    //     with the ray (initially infinity since there is no object).
    // (4) Normalize the ray.
    ray.org = pos;
    ray.dir = (2.0f * ((x / (float) resX - .5) * aspect) * xAxis) + // x
        (2.0f * (y / (float) resY - .5) * yAxis) + // y
        (focus * zAxis); // z
    ray.t = INFINITY_RAY;
    ray.recursionLevel = 0;
    ray.hit = NULL;
    normalize(ray.dir);

    return true;
  }

  virtual bool initRay(float x, float y, float offX, float offY, Ray &ray)
  {
    return this->initRay(x, y, ray);
  }

};

#endif
