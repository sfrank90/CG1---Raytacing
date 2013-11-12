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

    return true;
  }

};

#endif
