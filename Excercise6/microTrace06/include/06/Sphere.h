#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "../05/Primitive05.h"

class Sphere: public Primitive05
{
public:

  Vec3f center;
  float radius;

  Sphere(Vec3f center, float radius, Shader *shader)
      : Primitive05(shader), center(center), radius(radius)
  {
  }

  virtual ~Sphere()
  {
  }

  virtual string getClassName() const
  {
    return string("sphere06");
  }

  /**
   * Finds an intersection of the ray with the sphere.
   */
  virtual bool findIntersection(Ray &ray)
  {
    // Mathematical derivation, numerically not very stable, but simple
    // --> find roots of f(t) = ((o+td)-C)^2 - r^2
    // --> f(t) = (o-C)^2 + 2(o-C)(td) + (td)^2 -r^2
    // --> f(t) = [d^2] t^2 + [2d(o-C)] t + [(o-C)^2 - r^2]
    Vec3f diff = ray.org - center;
    float a = dot(ray.dir, ray.dir);
    float b = 2 * dot(ray.dir, diff);
    float c = dot(diff, diff) - radius * radius;

    // Use 'abc'-formula for finding root t_1,2 = (-b +/- sqrt(b^2-4ac))/(2a)
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
      return false;
    }
    float root = sqrt(discriminant);
    float dist = (-b - root) / (2 * a); // try t0.
    if (dist > ray.t)
    {
      return false;
    }

    // Test if t is closer than the current ray.t and if t is not zero.
    // If so, set the new ray.t and return true
    if (dist < EPSILON) // i.e. t0 behind camera.
    {
      dist = (-b + root) / (2 * a); // try t1.
      if (dist < EPSILON || dist > ray.t)
      {
        return false;
      }
    }
    ray.t = dist;
    ray.hit = this;

    return true;
  }

  /**
   * Returns the normal to the intersection point.
   */
  virtual Vec3f getNormal(Ray &ray)
  {
    Vec3f normal = (ray.org + ray.t * ray.dir - center);
    normalize(normal);
    return normal;
  }

  /**
   * Returns a bounding box of the sphere.
   */
  virtual Box calcBounds()
  {
    Box bounds;
    bounds.extendBox(center - Vec3f(radius, radius, radius));
    bounds.extendBox(center + Vec3f(radius, radius, radius));
    return bounds;
  }

};

#endif
