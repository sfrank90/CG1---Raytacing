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
    normalize(normal);
  }

  virtual string getClassName() const
  {
    return string("plane01");
  }

  virtual bool findIntersection(Ray &ray)
  {
    // Use your mathematical derivation from task 1.2a.
    //
    // Calculate a value for t, the distance of the intersected object on the
    // ray, mind the special case when ray.dir is parallel (within range) to
    // the plane!
    //
    // Test whether t is closer than the current ray.t and if t is not zero.
    // If so, set the new ray.t and return true.
    if (fabs(dot(ray.dir, normal)) < EPSILON)
    {
      return false;
    }
    Vec3f diff = origin - ray.org;
    float t = dot(diff, normal) / dot(ray.dir, normal);
    if (t < EPSILON || t > ray.t)
    {
      return false;
    }
    ray.t = t;

    return true;
  }

};

#endif
