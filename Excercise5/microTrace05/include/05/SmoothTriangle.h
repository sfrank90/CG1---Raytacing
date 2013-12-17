#pragma once
#ifndef SMOOTHTRIANGLE_H
#define SMOOTHTRIANGLE_H

#include "../Shader.h"
#include "Triangle.h"

class SmoothTriangle: public Triangle
{
  // Vec3f a, b, c; // vtx positions inherited from Triangle
  Vec3f na, nb, nc; // vertex normals

public:
  SmoothTriangle(Vec3f a, Vec3f b, Vec3f c, // vertices
      Vec3f na, Vec3f nb, Vec3f nc, // normals
      Shader *shader) // shaderr
      : Triangle(a, b, c, shader), na(na), nb(nb), nc(nc)
  {
  }

  virtual string getClassName() const
  {
    return string("smooth_triangle05");
  }

  virtual Vec3f getNormal(Ray &ray)
  {
    // assume u/v coordinates in ray correspond to beta(u) and gamma
    // (v) barycentric coordinates of hitpoint on triangle (have to be
    // stored like this in the intersection code !)
    Vec3f normal = ray.u * nb + ray.v * nc + (1 - ray.u - ray.v) * na;
    normalize(normal);
    return normal;
  }

};

#endif
