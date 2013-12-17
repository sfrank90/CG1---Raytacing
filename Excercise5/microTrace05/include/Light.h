#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Vec3f.h"
#include "Ray.h"

class Scene;
// forward declaration

class Light
{
protected:
  Scene *scene;

public:
  Light(Scene *scene)
      : scene(scene)
  {
  }

  virtual ~Light()
  {
  }

  /**
   * @param ray ray.org specifies the point to be illuminated.
   * @param intensity The light intensity at the illuminated point.
   */
  virtual bool illuminate(Ray &ray, Vec3f &intensity) = 0;

};

#endif
