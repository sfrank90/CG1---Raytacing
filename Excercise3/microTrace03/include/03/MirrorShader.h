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
	  Vec3f s_normal = ray.hit->getNormal(ray);

      // (2) Compute reflection vector.
	  reflection = 2 * (dot(s_normal, ray.dir)) * s_normal - ray.dir;

      // (3) Change ray direction and position.
	  ray.org = ray.org + ray.dir * ray.t;
	  ray.dir = -reflection;
	  
	  //PROBLEM: Output Bild 1 für den Shader schneidet die Spiegelungen auf dem Boden ab?!

      // (4) Trace the ray back into the scene.
	  result = scene->rayTrace(ray);
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
