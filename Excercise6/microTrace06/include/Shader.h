#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "Vec3f.h"
#include "Ray.h"

class Scene;
// forward declaration

class Shader
{
protected:
  Scene *scene;                   // scene that the shader is rendering.

public:
  unsigned int num_samples;       // number of samples that will be used for quad lights.
  unsigned int num_bounces;       // maximum recursion depth for path tracing.

  Shader(Scene *scene)
      : scene(scene), num_samples(1), num_bounces(0)
  {
  }

  virtual ~Shader()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Returns a color for the ray.
   */
  virtual Vec3f shadeRay(Ray &ray) = 0;

  /**
   * Returns the class name of the shader.
   */
  virtual string getClassName() const
  {
    return string("undefined");
  }

  // --------------------------------------------------------------------------

  /**
   * Determines whether rays that account for transparencey must be added.
   */
  virtual bool isTransparent()
  {
    return false;
  }

  /**
   * Returns the number of samples used for quad light.
   */
  unsigned int getSampleCount()
  {
    return num_samples;
  }

  /**
   * Sets the number of samples used for quad light.
   */
  void setSampleCount(unsigned int num_samples)
  {
    this->num_samples = num_samples;
  }

};

inline std::ostream &operator<<(std::ostream &o, const Shader &shader)
{
  o << "Shader[" << 5 << "]";
  return o;
}

#endif
