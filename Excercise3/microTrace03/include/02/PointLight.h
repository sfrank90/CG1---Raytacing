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
    // (1) Define the secondary ray direction from the surface point
    //     to the light source. The ray has not yet hit any object.
    //     Set t to the maximum length, i.e. almost the length of the
    //     distance from light source to surface point. Normalize the
    //     ray direction.
    // (2) Now compute the intensity of the illuminated surface.
    //     Remember: it is reciprocal to the squared distance.
    //     Multiply it with the light source's intensity.

    // Light ray direction towards point light position.
    lightray.dir = position - lightray.org;
    lightray.hit = NULL;
    lightray.t = length(lightray.dir) - EPSILON;
    normalize(lightray.dir);

    // Return attenuated intensity.
    float sqDist = lightray.t * lightray.t;
    float attenuation = 1.0 / sqDist; // due to point light.
    intensity = attenuation * this->intensity;

    return true;
  }

};

#endif
