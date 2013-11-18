#pragma once
#ifndef PRIMITIVE02_H
#define PRIMITIVE02_H

#include "../Primitive.h"
#include "../Light.h"

class Primitive02: public Primitive
{
protected:
  Vec3f color;
  Light *light;

public:
  Primitive02(Vec3f color, Light* light)
      : color(color), light(light)
  {
  }

  virtual ~Primitive02()
  {
  }

  virtual bool findIntersection(Ray &ray) = 0;

  virtual bool isOccluded(Ray &ray)
  {
    return findIntersection(ray);
  }

  virtual Vec3f getNormal(Ray &ray) = 0;

  virtual Vec3f getColor()
  {
    return color;
  }

  virtual Vec3f TraceLightRay(Ray &ray)
  {
    // IMPLEMENT ME 2.1
    // (1) Generate a secondary ray. Set the ray origin to the surface point
    //     that has been hit, but do not set direction quite yet.
    // (2) Call the illuminate method of your light source, store the intensity.
    // (3) Check whether the angle between surface normal and direction to
    //     light source is <= 90 deg (the surface is lit).
    //     If so, modulate the stored intensity with the cosine of the angle
    //     between surface normal and direction to light source.
    // IMPLEMENT ME END

    return intensity;
  }

};

#endif
