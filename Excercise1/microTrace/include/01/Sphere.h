#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Primitive01.h"

class Sphere: public Primitive01
{
  Vec3f center;
  float radius;

public:

  Sphere(Vec3f center, float radius)
      : center(center), radius(radius)
  {
  }

  virtual string getClassName() const
  {
    return string("sphere01");
  }

  virtual bool findIntersection(Ray &ray)
  {
    // IMPLEMENT ME 1.2
    // Use your mathematical derivation from task 1.2b.
    //
    // Calculate a value for t, the distance of the intersected object on
    // the ray.
    // IMPLEMENT ME END.

    Vec3f diff = ray.org - center;
    float a = dot(ray.dir,ray.dir);
    float b = 2 * dot(ray.dir,diff);
    float c = dot(diff,diff) - radius * radius;

    float inRoot = b*b - 4*a*c;
    if (inRoot < 0)
    {
      return false;
    }
    float root = sqrt(inRoot);

    float dist = (-b - root)/(2*a);
    if (dist > ray.t)
    {
      return false;
    }

    if (dist < EPSILON) {
      dist = (-b + root)/(2*a);
      if (dist < EPSILON || dist > ray.t)
      {
        return false;
      }
    }

    ray.t = dist;
    ray.hit = this;

    return true;
  }

};

#endif
