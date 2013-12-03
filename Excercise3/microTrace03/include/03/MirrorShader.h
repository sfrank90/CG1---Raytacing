#pragma once
#ifndef MIRRORSHADER_H
#define MIRRORSHADER_H MIRRORSHADER_H

#include "../Shader.h"

class MirrorShader: public Shader
{
private:
  Vec3f reflection;

public:
  MirrorShader(Scene *scene, const Vec3f& reflection)
      : Shader(scene), reflection(reflection)
  {
  }

  virtual string getClassName() const
  {
    return string("mirror03");
  }

  virtual Vec3f shadeRay(Ray &ray)
  {
    if (++ray.recursionLevel < MAX_RECURSION_LEVEL)
    {
      Vec3f result;

      // IMPLEMENT ME 3.1
      // (1) Get normal of the hit surface.
      // (2) Compute reflection vector.
      // (3) Change ray direction and position.
      // (4) Trace the ray back into the scene.
      // IMPLEMENT ME END

      return result;
    }
    else
    {
      return Vec3f(0);
    }
  }

};

#endif
