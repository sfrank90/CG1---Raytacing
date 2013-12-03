#pragma once
#ifndef EYELIGHTSHADER_H
#define EYELIGHTSHADER_H

#include "../Shader.h"

class EyeLightShader: public Shader
{
public:
  Vec3f color;

  EyeLightShader(Scene *scene, Vec3f color = Vec3f(0.5, 0.5, 0.5))
      : Shader(scene), color(color)
  {
  }

  virtual string getClassName() const
  {
    return string("eyelight03");
  }

  virtual Vec3f shadeRay(Ray &ray)
  {
    Vec3f result = fabs(dot(ray.hit->getNormal(ray), ray.dir)) * color;
    return result;
  }

};

#endif
