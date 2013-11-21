#pragma once
#ifndef PRIMITIVE02_H
#define PRIMITIVE02_H

#include "../Primitive.h"
#include "../Light.h"

class Primitive02: public Primitive
{
protected:
  Vec3f color;
  Light *light;

public:
  Primitive02(Vec3f color, Light* light)
      : color(color), light(light)
  {
  }

  virtual ~Primitive02()
  {
  }

  virtual bool findIntersection(Ray &ray) = 0;

  virtual bool isOccluded(Ray &ray)
  {
    return findIntersection(ray);
  }

  virtual Vec3f getNormal(Ray &ray) = 0;

  virtual Vec3f getColor()
  {
    return color;
  }

  virtual Vec3f TraceLightRay(Ray &ray)
  {
    // IMPLEMENT ME 2.1
    // (1) Generate a secondary ray. Set the ray origin to the surface point
    //     that has been hit, but do not set direction quite yet.   
    Ray r2 = Ray();
    r2.org = ray.org + ray.dir * ray.t;

    // (2) Call the illuminate method of your light source, store the intensity.
    Vec3f intensity = (0.f);
    light->illuminate(r2, intensity);

    // (3) Check whether the angle between surface normal and direction to
    //     light source is <= 90 deg (the surface is lit).
    //     If so, modulate the stored intensity with the cosine of the angle
    //     between surface normal and direction to light source.

    Vec3f s_normal = ray.hit->getNormal(ray); //Error located here, wrong ray used to get correct normal...
	
	//dir and normal are already normalized!
    float cos_angle = dot(s_normal, r2.dir);

   /* if(cos_angle >= 0) //depricated, see below
        intensity = intensity * cos_angle;
    else
        intensity = (0.f);*/
    
	//Result
	return max(0.0, cos_angle)*componentProduct(intensity, color);

    // IMPLEMENT ME END
  }

};

#endif
