#pragma once
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Primitive03.h"
#include "../Shader.h"

class Triangle: public Primitive03
{
public:
	Vec3f a, b, c;

  Triangle(Vec3f a, Vec3f b, Vec3f c, Shader *shader)
      : Primitive03(shader), a(a), b(b), c(c)
  {
  }

  virtual string getClassName() const
  {
    return string("triangle03");
  }

  virtual bool findIntersection(Ray &ray)
  {
    const Vec3f edge1 = b - a;
    const Vec3f edge2 = c - a;

    const Vec3f pvec = ray.dir ^ edge2;

    const float det = dot(edge1, pvec);
    if (fabs(det) < EPSILON)
    {
      return false;
    }

    const float inv_det = 1.0f / det;

    const Vec3f tvec = ray.org - a;
    float lambda = dot(tvec, pvec);
    lambda *= inv_det;

    if (lambda < 0.0f || lambda > 1.0f)
    {
      return false;
    }

    const Vec3f qvec = tvec ^ edge1;
    float mue = dot(ray.dir, qvec);
    mue *= inv_det;

    if (mue < 0.0f || mue + lambda > 1.0f)
    {
      return false;
    }

    float f = dot(edge2, qvec);
    f *= inv_det;
    if (ray.t <= f || f < 1E-4)
    {
      return false;
    }

    ray.t = f;
    ray.hit = this;
    ray.u = lambda;
    ray.v = mue;
    return true;
  }

  virtual Vec3f getNormal(Ray &ray)
  {
    Vec3f normal = cross((b - a), (c - a));
    normalize(normal);
    return normal;
  }

};

#endif
