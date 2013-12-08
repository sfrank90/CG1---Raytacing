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
      Vec3f result, rv;

      // IMPLEMENT ME 3.1
      // (1) Get normal of the hit surface.
	  Vec3f s_normal = ray.hit->getNormal(ray);

      // (2) Compute reflection vector.

	  rv = 2 * (dot(s_normal, ray.dir)) * s_normal - ray.dir;
	  normalize(rv);

      // (3) Change ray direction and position.
	  ray.org = ray.org + ray.dir * ray.t;
	  ray.dir = -rv;    //
	  ray.t = INFINITY; // 
	  ray.hit = NULL;   // Nothing hitted yet
	  
	  //PROBLEM: Output Bild 1 für den Shader schneidet die Spiegelungen auf dem Boden ab?!

      // (4) Trace the ray back into the scene.
	  result = componentProduct(reflection,scene->rayTrace(ray)); // "reflection" controls, what should be exactly reflected, (1,1,1) means ideal reflection like a mirror
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
