#pragma once
#ifndef REFERACTIVESHADER_H
#define REFERACTIVESHADER_H

#include "../Shader.h"
#include "../Scene.h"

class RefractiveShader: public Shader
{
public:
  Vec3f color;
  double refractivity;
  double indexOutside;
  double indexInside;

  RefractiveShader(Scene *scene, Vec3f color = Vec3f(0.5, 0.5, 0.5), // bg
  double refractivity = 0.4, double indexInside = 1, double indexOutside = 1) // refraction coefficients.
      : Shader(scene), color(color), refractivity(refractivity), indexOutside(indexOutside), indexInside(indexInside)
  {
  }

  virtual string getClassName() const
  {
    return string("refractive04");
  }

  virtual bool isTransparent()
  {
    return true;
  }

  virtual Vec3f shadeRay(Ray &ray)
  {
    Vec3f ret_val = Vec3f(0);

    if (ray.recursionLevel <= MAX_RECURSION_LEVEL)
    {
      // IMPLEMENT ME 4.1
      // (1) Calculate refractive index.
      // (2) Get shading normal and look from where the ray is coming.
      //     Modify normal and refractive index when necessary.
      // (3) Calculate the refracted ray direction.
      // (4) Make cases for refraction and total reflection.
      // IMPLEMENT ME END

    }
    return ret_val;
  }

};

#endif
