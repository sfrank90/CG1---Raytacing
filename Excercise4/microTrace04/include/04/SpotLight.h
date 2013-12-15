#pragma once
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "../Light.h"

class SpotLight: public Light
{
private:
  Vec3f m_pos, m_dir;                   // position and direction
  Vec3f m_intensity;                    // emission (red, green, blue)
  float m_alpha_min, m_alpha_max;       // opening angles

public:
  SpotLight(Scene* scene, // scene
      const Vec3f& pos, const Vec3f& dir, const Vec3f& intensity, // position, direction, power
      float alpha_min, float alpha_max) // in degree
      : Light(scene), m_pos(pos), m_dir(dir), m_intensity(intensity), m_alpha_min(alpha_min), m_alpha_max(alpha_max)
  {
    normalize(m_dir);
  }

  virtual ~SpotLight()
  {
  }

  const Vec3f& position() const
  {
    return m_pos;
  }

  const Vec3f& direction() const
  {
    return m_dir;
  }

  virtual bool illuminate(Ray& shadow_ray, Vec3f& intensity)
  {
    // Direction vector from light source to surface point,
    // and angle between light source dir and shadow ray dir.
    Vec3f d_norm = shadow_ray.org - m_pos;
    normalize(d_norm);
    float alpha = fabs(acos(dot(d_norm, m_dir)) * 180 / M_PI);

    // No light outside cone.
    if (alpha > m_alpha_max)
    {
      return false;
    }

    // Falloff for distance between light and illuminated point.
    Vec3f dir = m_pos - shadow_ray.org;
    float r = length(dir) - EPSILON;
    float falloff = 1.0f / (r * r); // falloff for distance

    // Set shadow ray with direction to light source.
    normalize(dir);
    shadow_ray.hit = NULL;
    shadow_ray.t = r;
    shadow_ray.dir = dir;

    // IMPLEMENT ME 8.2
    // (1) Assign full intensity (w.r.t. to the squared-distance-falloff).
    // (2) Or assign linear falloff from 1 at alpha_min to 0 at alpha_max.
    // IMPLEMENT ME END

    return true;
  }

};

#endif
