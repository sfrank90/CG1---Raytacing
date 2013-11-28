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
    // Generate a secondary ray and set the ray origin to
    // the surface point that has been hit.
    Ray lightray;
    lightray.org = ray.org + ray.dir * ray.t;

    // Store the intensity of the incoming light.
    Vec3f intensity;
    light->illuminate(lightray, intensity);

    // Check whether the angle between surface normal and direction
    // to light source is <= 90 deg (i.e. the surface is lit).
    float cos_wi = dot(this->getNormal(ray), lightray.dir);
    if (cos_wi > 0)
    {
      // If so, modulate the stored intensity with the cosine of the angle
      // between surface normal and direction to light source.
      intensity *= cos_wi;
    }
    else
    {
      intensity *= 0; // intensity is vec3.
    }
    intensity *= this->getColor();
    return intensity;
  }

};

#endif
