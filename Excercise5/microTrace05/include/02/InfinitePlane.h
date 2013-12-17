#pragma once
#ifndef INFINITEPLANE_H
#define INFINITEPLANE_H

#include "Primitive02.h"
#include "../Light.h"

class InfinitePlane: public Primitive02
{
  Vec3f normal;
  Vec3f origin;

public:
  InfinitePlane(Vec3f origin, Vec3f normal, Light *light, Vec3f color)
      : Primitive02(color, light), normal(normal), origin(origin)
  {
    normalize(normal);
  }

  virtual string getClassName() const
  {
    return string("plane02");
  }

  virtual bool findIntersection(Ray &ray)
  {
    Vec3f diff = origin - ray.org;
    float t = dot(diff, normal) / dot(ray.dir, normal);
    if (t < EPSILON || t > ray.t)
    {
      return false;
    }
    ray.t = t;
    ray.hit = this;
    return true;
  }

  virtual Vec3f getNormal(Ray &ray)
  {
    return normal;
  }

};

#endif
