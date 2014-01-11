#pragma once
#ifndef BOX_H
#define BOX_H

#include "../Ray.h"

class Box
{
public:
  Vec3f min, max;

  Box()
  {
    clearBox();
  }

  /**
   * Initialize the sides of the box with -infinity
   * and +inifinity, respectively.
   */
  inline void clearBox()
  {
    min = Vec3f(INFINITY_RAY, INFINITY_RAY, INFINITY_RAY);
    max = Vec3f(-INFINITY_RAY, -INFINITY_RAY, -INFINITY_RAY);
  }

  /**
   * Extends the current box to hold in a point.
   */
  inline void extendBox(Vec3f a)
  {
    // Set the minimum and maximum so that a is inside.
    min = minComponent(a, min);
    max = maxComponent(a, max);
  }

  /**
   * Extends the current box to hold in another box.
   */
  inline void extendBox(Box box)
  {
    extendBox(box.min);
    extendBox(box.max);
  }

  /**
   * Determines whether this box overlaps with another one.
   */
  inline bool overlapsBox(Box &b)
  {
    // Determine whether the boxes overlap.
    for (int i = 0; i < 3; i++)
    {
      if (b.min[i] - EPSILON > max[i]) return false;
      if (b.max[i] + EPSILON < min[i]) return false;
    }
    return true;
  }

  /**
   * Sets the near and far clipping point of a ray
   * with this box into t_near and t_far.
   */
  void clipBox(Ray &ray, float &tnear, float &tfar)
  {
    // Determine near and far point of the ray cutting the box.
    float d, den;

    // Cutting on x side?
    den = 1. / ray.dir.x;
    if (ray.dir.x > 0)
    {
      if ((d = (min.x - ray.org.x) * den) > tnear) tnear = d;
      if ((d = (max.x - ray.org.x) * den) < tfar) tfar = d;
    }
    else
    {
      if ((d = (max.x - ray.org.x) * den) > tnear) tnear = d;
      if ((d = (min.x - ray.org.x) * den) < tfar) tfar = d;
    }
    if (tnear > tfar) return;

    // Cutting on y side?
    den = 1. / ray.dir.y;
    if (ray.dir.y > 0)
    {
      if ((d = (min.y - ray.org.y) * den) > tnear) tnear = d;
      if ((d = (max.y - ray.org.y) * den) < tfar) tfar = d;
    }
    else
    {
      if ((d = (max.y - ray.org.y) * den) > tnear) tnear = d;
      if ((d = (min.y - ray.org.y) * den) < tfar) tfar = d;
    }
    if (tnear > tfar) return;

    // Cutting on z side?
    den = 1. / ray.dir.z;
    if (ray.dir.z > 0)
    {
      if ((d = (min.z - ray.org.z) * den) > tnear) tnear = d;
      if ((d = (max.z - ray.org.z) * den) < tfar) tfar = d;
    }
    else
    {
      if ((d = (max.z - ray.org.z) * den) > tnear) tnear = d;
      if ((d = (min.z - ray.org.z) * den) < tfar) tfar = d;
    }
  }

};

#endif
