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
    // IMPLEMENT ME 5.1a
    // Set the minimum and maximum so that a is inside.
    // IMPLEMENT ME END
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
    // IMPLEMENT ME 5.1a
    // Determine whether the boxes overlap.
    // IMPLEMENT ME END
    return true;
  }

  /**
   * Sets the near and far clipping point of a ray
   * with this box into t_near and t_far.
   */
  void clipBox(Ray &ray, float &tnear, float &tfar)
  {
    // IMPLEMENT ME 5.1a
    // Determine near and far point of the ray cutting the box.
    // IMPLEMENT ME END
  }

};

#endif
