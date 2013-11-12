#pragma once
#ifndef RAY_H
#define RAY_H

#include "Vec3f.h"

class Primitive;
// forward declaration

class Ray
{
public:
  Vec3f org;                    // origin
  Vec3f dir;                    // direction
  float t;                      // current/maximum hit distance
  Primitive *hit;               // currently closest primitive
  float u, v;                   // barycentric u/v coordinates
  unsigned int recursionLevel;  // number of light ray bounces

  Ray()
      : org(Vec3f(0.f)), dir(Vec3f(0.f)), // origin, direction
      t(INFINITY_RAY), hit(NULL),             // hit distance and primitive
      u(0.f), v(0.f),                     // barycentric coords
      recursionLevel(0)                   // number of light ray bounces
  {
  }

  Ray(Vec3f origin, Vec3f direction)
      : org(origin), dir(direction),      // origin, direction
      t(INFINITY_RAY), hit(NULL),             // hit distance and primitive
      u(0.f), v(0.f),                     // barycentric coords
      recursionLevel(0)                   // number of light ray bounces
  {
  }

};

inline std::ostream &operator<<(std::ostream &o, const Ray &ray)
{
  o << "Ray[" << ray.org << "+t*" << ray.dir << "]";
  return o;
}

#endif
