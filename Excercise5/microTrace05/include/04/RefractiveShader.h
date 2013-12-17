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
      // (1) Calculate refractive index.
      // (2) Get shading normal and look from where the ray is coming.
      //     Modify normal and refractive index when necessary.
      // (3) Calculate the refracted ray direction.
      // (4) Make cases for refraction and total reflection.

      // Calculate refractive index.
      float eta = indexInside / indexOutside;

      // Get normal, look from where the ray is coming.
      Vec3f normal = ray.hit->getNormal(ray);
      normalize(normal);
      if (dot(normal, ray.dir) > 0)
      {
        normal = -normal;
        eta = indexOutside / indexInside;
      }

      // Hit point and incoming ray direction.
      Vec3f p0 = ray.org + ray.t * ray.dir;
      Vec3f d_i = ray.dir;
      normalize(d_i);

      // Calculate the refracted ray.
      float cos_w_i = dot(normal, -d_i);
      float cos_w_o = sqrt(1 + eta * eta * (cos_w_i * cos_w_i - 1));
      Vec3f d_o = eta * d_i + (eta * cos_w_i - cos_w_o) * normal;

      // Check whether it is a refraction.
      if (dot(d_o, normal) <= 0.0)
      {
        Ray refract_ray = Ray(ray);
        refract_ray.org = p0;
        refract_ray.dir = d_o;
        refract_ray.t = INFINITY_RAY;
        ret_val = scene->rayTrace(refract_ray);
      }

      // Otherwise, it is a total reflection.
      else
      {
        Ray reflect_ray = Ray(ray);
        reflect_ray.org = p0;
        reflect_ray.dir = d_i - 2 * (dot(normal, d_i)) * normal;
        reflect_ray.t = INFINITY_RAY;
        reflect_ray.recursionLevel++;

        // Return mirrored color.
        ret_val = scene->rayTrace(reflect_ray);
        ret_val = componentProduct(ret_val, Vec3f(refractivity));
        ret_val = componentProduct(ret_val, color);
      }

    }
    return ret_val;
  }

};

#endif
