#pragma once
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Primitive01.h"

class Triangle: public Primitive01
{
  Vec3f a, b, c;

public:
  Triangle(Vec3f a, Vec3f b, Vec3f c)
      : a(a), b(b), c(c)
  {
  }

  virtual string getClassName() const
  {
    return string("triangle01");
  }

  virtual bool findIntersection(Ray &ray)
  {
    const Vec3f edge1 = b - a;
    const Vec3f edge2 = c - a;

    const Vec3f pvec = ray.dir ^ edge2;

    const float det = dot(edge1, pvec);
    if (fabs(det) < EPSILON) return false;

    const float inv_det = 1.0f / det;

    const Vec3f tvec = ray.org - a;
    float lambda = dot(tvec, pvec);
    lambda *= inv_det;

    if (lambda < 0.0f || lambda > 1.0f) return false;

    const Vec3f qvec = tvec ^ edge1;
    float mue = dot(ray.dir, qvec);
    mue *= inv_det;

    if (mue < 0.0f || mue + lambda > 1.0f) return false;

    float f = dot(edge2, qvec);
    f *= inv_det;
    if (ray.t <= f || f < 1E-4) return false;

    ray.t = f;
    ray.hit = this;

    return true;
  }

};

#endif
