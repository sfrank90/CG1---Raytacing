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
	Vec3f result = Vec3f(0.0f);
    // (1) Get shading normal and turn normal to front if necessary.
	Vec3f s_normal = ray.hit->getNormal(ray);

	if (dot(s_normal,ray.dir) > 0)
			s_normal = -s_normal;
    // (2) Calculate ambient term (intensity = 0).
	Vec3f rad_amb = Vec3f(1.0,1.0,1.0);
	Vec3f intensity_high = Vec3f(0.0f);
	Vec3f ambient_term =  componentProduct(intensity_high, rad_amb);
    
	// (3) Start shadow ray from surface point and iterate over all light
    //     sources, using phi and theta1/theta2.
	Ray shadow_ray = Ray();
	shadow_ray.org = ray.org + ray.dir * ray.t;

	for (std::vector<Light*>::iterator l = scene->lights.begin(); l != scene->lights.end(); ++l) {
		Vec3f intensity;
		if((*l)->illuminate(shadow_ray, intensity) && !scene->isOccluded(shadow_ray)) {
			if(dot(shadow_ray.dir, s_normal) > 0) {
				//calculate theta_in, fi_in, theta_out, fi_out to get color
				double theta_in = acos(dot(-ray.dir, s_normal)); //easy
				double fi_in = 0.0; //easy
				double theta_out = acos(dot(shadow_ray.dir, s_normal)); //easy
				double fi_out; //somehow complex...

				//create local coordiante system
				Vec3f x = cross(-ray.dir, s_normal);
				Vec3f y = cross(s_normal, x);
				normalize(x);
				normalize(y);

				//project light direction into coordinate system
				Vec3f l_xy = cross(shadow_ray.dir, s_normal);
				normalize(l_xy);

				//use cosine and sine as arguments to calculate fi_out by using the tangens
				fi_out = atan2(dot(l_xy, y), dot(l_xy, x));
				double r,g,b;
				read->lookup_brdf_val(theta_in, fi_in, theta_out, fi_out, r, g, b);

				result += componentProduct(scale,componentProduct(Vec3f((float)r,(float)g,(float)b), intensity))*dot(shadow_ray.dir, s_normal);
			}
		}
	}
    // (4) Abort if the shadow ray is occluded.
    // IMPLEMENT ME END


    return (result+ambient_term);
  }

};

#endif
