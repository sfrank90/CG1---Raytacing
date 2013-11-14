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
    // IMPLEMENT ME 1.1
    // Use your mathematical derivation from task 1.1.
    //
    // (1) Calculate zAxis, yAxis and xAxis.
	// (2) Normalize them.
	zAxis = pos - dir;
	xAxis = cross(up, zAxis);
	yAxis = cross(zAxis, xAxis); //this calculation should be right? tested inverted direction -> primitives are not drawn
	
	Normalize(xAxis);
	Normalize(zAxis);
	Normalize(yAxis);
	
    // (3) Calculate the aspect ratio.
	aspect = (float)resX/(float)resY;//length(2 * tan(angle) * dir);

    // (4) Calculate the opening angle in radians.
	float radAngle = angle * (M_PI/180.);

    // (5) Calculate the focus (distance to plane in pixels).
	focus = 1.f / tan(radAngle/2.f);//length(dir);
    // IMPLEMENT ME END
  }

  virtual ~PerspectiveCamera()
  {
  }

  virtual bool initRay(float x, float y, Ray &ray)
  {
    // IMPLEMENT ME 1.4
    // Use your mathematical derivation from task 1.2.
    //
    // (1) Calculate the origin of the ray.
    // (2) Calculate the direction of the ray.
    // (3) Set the current distance of the nearest object that intersects
    //     with the ray (initially infinity since there is no object).
    // (4) Normalize the ray.
    // IMPLEMENT ME END.
    ray.org = pos;
    ray.dir  =  2.0f * xAxis *(x/(float)resX - .5)*aspect
             +  2.0f * yAxis* (y/(float)resY - .5)
             -  focus * zAxis;
    ray.t = INFINITY_RAY;
    ray.recursionLevel=0;
    ray.hit = NULL;
    Normalize(ray.dir);

    return true;
  }

  virtual bool initRay(float x, float y, float offX, float offY, Ray &ray)
  {
    return this->initRay(x, y, ray);
  }

};

#endif
