#pragma once
#ifndef CHECKEREDINFINITEPLANE_H
#define CHECKEREDINFINITEPLANE_H

#include "../05/Primitive05.h"
#include "../Shader.h"

class CheckeredPlane: public Primitive05
{

public:

  Vec3f origin;
  Vec3f normal;
  Vec3f direction;

  CheckeredPlane(Vec3f origin, Vec3f normal, Vec3f direction, Shader *shader)
      : Primitive05(shader), origin(origin), normal(normal), direction(direction)
  {
    normalize(direction);
    normalize(normal);
  }

  virtual string getClassName() const
  {
    return string("rotated_plane06");
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

  virtual Box calcBounds()
  {
    Box bounds;
    if (normal.x == 0 && normal.y == 0)
    {
      bounds.extendBox(Vec3f(-INFINITY_RAY, -INFINITY_RAY, origin.z));
      bounds.extendBox(Vec3f(INFINITY_RAY, INFINITY_RAY, origin.z));
    }
    else if (normal.x == 0 && normal.z == 0)
    {
      bounds.extendBox(Vec3f(-INFINITY_RAY, origin.y, -INFINITY_RAY));
      bounds.extendBox(Vec3f(INFINITY_RAY, origin.y, INFINITY_RAY));
    }
    else if (normal.y == 0 && normal.z == 0)
    {
      bounds.extendBox(Vec3f(origin.x, -INFINITY_RAY, -INFINITY_RAY));
      bounds.extendBox(Vec3f(origin.x, INFINITY_RAY, INFINITY_RAY));
    }
    return bounds;
  }

};

#endif
