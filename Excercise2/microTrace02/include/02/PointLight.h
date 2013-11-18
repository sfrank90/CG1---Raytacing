#pragma once
#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "../Light.h"

class PointLight: public Light
{

public:
  Vec3f position;  // origin
  Vec3f intensity; // emission (red, green, blue)

  PointLight(Scene *scene, Vec3f position, Vec3f intensity)
      : Light(scene), position(position), intensity(intensity)
  {
  }

  /**
   * Given a light ray with origin already set, but not yet direction,
   * computes the light intensity.
   */
  virtual bool illuminate(Ray &lightray, Vec3f &intensity)
  {
    // IMPLEMENT ME 2.2
    // (1) Define the secondary ray direction from the surface point
    //     to the light source. The ray has not yet hit any object.
    //     Set t to the maximum length, i.e. almost the length of the
    //     distance from light source to surface point. Normalize the
    //     ray direction.
    // (2) Now compute the intensity of the illuminated surface.
    //     Remember: it is reciprocal to the squared distance.
    //     Multiply it with the light source's intensity.
    // IMPLEMENT ME END

    return true;
  }

};

#endif
