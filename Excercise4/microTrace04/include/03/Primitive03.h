#pragma once
#ifndef PRIMITIVE03_H
#define PRIMITIVE03_H

#include "../Primitive.h"

class Primitive03: public Primitive
{
public:

  Primitive03(Shader *shader = NULL)
      : Primitive(shader)
  {
  }

  virtual ~Primitive03()
  {
  }

  virtual Vec3f getNormal(Ray &ray) = 0;

  virtual bool findIntersection(Ray &ray) = 0;

  virtual bool isOccluded(Ray &ray)
  {
    return findIntersection(ray);
  }

  virtual bool isTransparent()
  {
    return shader->isTransparent();
  }

  virtual void getUV(Ray &ray, float &u, float &v)
  {
    u = ray.org.x + ray.t * ray.dir.x;
    v = ray.org.z + ray.t * ray.dir.z;
  }

};

#endif
