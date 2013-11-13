#pragma once
#ifndef INFINITEPLANE_H
#define INFINITEPLANE_H

#include "Primitive01.h"

class InfinitePlane: public Primitive01
{
  Vec3f normal;
  Vec3f origin;

public:

  InfinitePlane(Vec3f origin, Vec3f normal)
      : normal(normal), origin(origin)
  {
    Normalize(normal);
  }

  virtual string getClassName() const
  {
    return string("plane01");
  }

  virtual bool findIntersection(Ray &ray)
  {
    // IMPLEMENT ME 1.2
    // Use your mathematical derivation from task 1.2a.
    //
    // Calculate a value for t, the distance of the intersected object on the
    // ray, mind the special case when ray.dir is parallel (within range) to
    // the plane!
    //
    // Test whether t is closer than the current ray.t and if t is not zero.
    // If so, set the new ray.t and return true.
    // IMPLEMENT ME END.

    Vec3f diff = origin - ray.org;
    float t = dot(diff,normal) / dot(ray.dir,normal);
    if (t < EPSILON || t > ray.t)
    {
      return false;
    }
    ray.t = t;
    ray.hit = this;
    return true;
  }

};

#endif
