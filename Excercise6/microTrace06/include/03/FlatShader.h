#pragma once
#ifndef FLATSHADER_H
#define FLATSHADER_H

#include "../Shader.h"

class FlatShader: public Shader
{
public:
  Vec3f color;

  FlatShader(Scene *scene, Vec3f color = Vec3f(0.5, 0.5, 0.5))
      : Shader(scene), color(color)
  {
  }

  virtual string getClassName() const
  {
    return string("flat03");
  }

  virtual Vec3f shadeRay(Ray &ray)
  {
    return color;
  }

};

#endif
