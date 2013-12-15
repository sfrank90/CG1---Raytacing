#pragma once
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Ray.h"

class Shader;
// forward declaration.

class Primitive
{
public:

  Shader *shader;

  Primitive(Shader *shader = NULL)
      : shader(shader)
  {
  }

  virtual ~Primitive()
  {
  }

  // -------------------------------------------------------------------------

  /**
   * Saves the primitive intersection point in the ray, and returns whether an
   * intersection took place.
   */
  virtual bool findIntersection(Ray &ray) = 0;

  /**
   * Returns the class name of the shader.
   */
  virtual string getClassName() const
  {
    return string("undefined");
  }

  // -------------------------------------------------------------------------

  /**
   * Indicates whether the ray to the primitive is occluded.
   */
  virtual bool isOccluded(Ray &ray)
  {
    return false;
  }

  /**
   * Indicates whether the primitive is transparent.
   */
  virtual bool isTransparent()
  {
    return false;
  }

  /**
   * Returns the normal of the primitive.
   */
  virtual Vec3f getNormal(Ray &ray)
  {
    return Vec3f(0.f);
  }

};

inline std::ostream &operator<<(std::ostream &o, const Primitive &prim)
{
  o << "Primitive[" << prim.getClassName() << "]";
  return o;
}

#endif
