#pragma once
#ifndef BRDFSHADER_H
#define BRDFSHADER_H

#include "../Shader.h"
#include "BRDFRead.h"

class BRDFShader: public Shader
{

private:
  BRDFRead *read;
  Vec3f scale;

public:

  BRDFShader(Scene *scene, const char* file, Vec3f scale)
      : Shader(scene), scale(scale)
  {
    read = new BRDFRead(file);
  }

  virtual ~BRDFShader()
  {
  }

  virtual string getClassName() const
  {
    return string("brdf03");
  }

  virtual bool isTransparent()
  {
    return false;
  }

  virtual Vec3f shadeRay(Ray &ray)
  {
    // IMPLEMENT ME 3.3
    // (1) Get shading normal and turn normal to front if necessary.
    // (2) Calculate ambient term (intensity = 0).
    // (3) Start shadow ray from surface point and iterate over all light
    //     sources, using phi and theta1/theta2.
    // (4) Abort if the shadow ray is occluded.
    // IMPLEMENT ME END
	Vec3f result;

    return result;
  }

};

#endif
