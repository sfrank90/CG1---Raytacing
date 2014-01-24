#pragma once
#ifndef CHECKEREDEYELIGHTSHADER_H
#define CHECKEREDEYELIGHTSHADER_H

#include "../Shader.h"
#include "../03/Primitive03.h"
#include "CheckeredPlane.h"

/**
 * Warning: This shader works only in conjunction with
 * a rotated infinite plane.
 */
class CheckeredEyeLightShader: public Shader
{
public:
  float scale;
  Vec3f color1, color2;

  CheckeredEyeLightShader(Scene *scene, float scale, Vec3f color1 = Vec3f(.3), Vec3f color2 = Vec3f(.7))
      : Shader(scene), scale(scale), color1(color1), color2(color2)
  {
  }

  virtual string getClassName() const
  {
    return string("checkered_eyelight06");
  }

  virtual Vec3f shadeRay(Ray &ray)
  {
    CheckeredPlane *rip = (CheckeredPlane*) ray.hit;
    Vec3f p = ray.org + ray.t * ray.dir;
    Vec3f d = p - rip->origin;
    if (d == Vec3f(0))
    {
      return color2;
    }
    float u = dot(d, rip->direction) / scale;
    float v = dot(d, cross(rip->direction, rip->normal)) / scale;
    if (u < 0)
    {
      u = -u + .5;
    }
    if (v < 0)
    {
      v = -v + .5;
    }
    float rx = (u - floor(u));
    float ry = (v - floor(v));
    if (rx > 1.f || rx < 0.f || ry > 1.f || ry < 0.f)
    {
      printf("This should not happen: u=%.2f, v=%.2f, rx=%.4f, ry=%.4f\n", u, v, rx, ry);

    }
    Vec3f color = (rx < .5 && ry < .5) || (rx > .5 && ry > .5) ? color1 : color2;
    // color *= fabs(dot(rip->getNormal(ray), ray.dir));
    return color;
  }

};

#endif
