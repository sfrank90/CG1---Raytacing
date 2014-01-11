#pragma once
#ifndef EYELIGHTSHADER_H
#define EYELIGHTSHADER_H

#include "../Shader.h"
#include "Primitive03.h"

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
    Vec3f normal = ((Primitive03*) ray.hit)->getNormal(ray);
    Vec3f result = fabs(dot(normal, ray.dir)) * color;
    return result;
  }

};

#endif
