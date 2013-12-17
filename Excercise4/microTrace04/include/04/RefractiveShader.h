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
		float n, n1 = indexInside, n2 = indexOutside;

      // (2) Get shading normal and look from where the ray is coming.
      //     Modify normal and refractive index when necessary
		Vec3f normal = ray.hit->getNormal(ray);

		if(dot(ray.dir, normal) > 0) {
			normal = -normal;
			n1 = indexOutside;
			n2 = indexInside;
		}
		n = n1/n2;

      // (3) Calculate the refracted ray direction.
		float c1 = dot(-ray.dir, normal);
		float c2_sq = 1.0f-n*n*(1-c1*c1);

      // (4) Make cases for refraction and total reflection.
		Ray reflection;
		reflection.org = ray.org + ray.t * ray.dir;
	    reflection.dir = ray.dir - 2*dot(normal,ray.dir)*normal;;
		reflection.t = INFINITY;
		reflection.hit = NULL;
		Vec3f ray_refl = scene->rayTrace(reflection);
		
		if(c2_sq < 0) { //total internal reflexion
			return componentProduct(Vec3f(refractivity),componentProduct(color,ray_refl));
		}

		ray.org = ray.org + ray.t * ray.dir;
		ray.dir = (n * c1 - sqrt(c2_sq)) * normal + ray.dir * n;
		ray.t = INFINITY;
		ray.hit = NULL;
		ray.recursionLevel++;

		/* solving the fresnel equation */
		if(true) { //sieht besser aus :>
			n = 1.0f / n;
			float g = sqrt(n * n + c1 * c1 - 1.0f);
			float F = 0.5f * pow(g - c1, 2) / pow(g + c1, 2) * (1.0f + pow((c1 * (g + c1) - 1.0f), 2)) / pow((c1 * (g - c1) + 1.0f), 2);

			float T = 1.0f / (1.0f + F); //wiki
			float R = 1.0f - T;
			n = 1.0f / n;

			ret_val = R*ray_refl;
			ret_val += T * scene->rayTrace(ray);
		} else {
			ret_val += scene->rayTrace(ray);
		}

      // IMPLEMENT ME END

    }
    return ret_val;
  }

};

#endif
