#pragma once
#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "../Shader.h"

class PhongShader: public Shader
{
public:
  Vec3f color; // base color
  float ka;    // ambient coefficient
  float kd;    // diffuse reflection coefficients
  float ks;    // specular refelection coefficients
  float ke;    // shininess exponent

  PhongShader(Scene *scene, Vec3f color, float ka, float kd, float ks, float ke)
      : Shader(scene), color(color), ka(ka), kd(kd), ks(ks), ke(ke)
  {
  }

  virtual string getClassName() const
  {
    return string("phong03");
  }

  virtual Vec3f shadeRay(Ray &ray)
  {
    Vec3f result;

    // IMPLEMENT ME 3.2
    // (1) Get shading normal and turn normal to front if necessary.
    // (2) Calculate ambient term (intensity = 1).
    // (3) Start shadow ray from surface point and iterate over all light
    //     sources (diffuse and specular term).
    // (4) Abort if the shadow ray is occluded.
    // IMPLEMENT ME END

    return result;
  }

};

#endif
