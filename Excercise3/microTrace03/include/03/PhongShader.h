#pragma once
#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include <typeinfo>
#include "../Shader.h"

class PhongShader: public Shader
{
public:
  Vec3f color; // base color
  float ka;    // ambient coefficient
  float kd;    // diffuse reflection coefficients
  float ks;    // specular refelection coefficients
  float ke;    // shininess exponent

  PhongShader(Scene *scene, Vec3f color, float ka, float kd, float ks, float ke)
      : Shader(scene), color(color), ka(ka), kd(kd), ks(ks), ke(ke)
  {
  }

  virtual string getClassName() const
  {
    return string("phong03");
  }

  virtual Vec3f shadeRay(Ray &ray)
  {
	Vec3f result = Vec3f(0.0, 0.0, 0.0);

    // IMPLEMENT ME 3.2
    // (1) Get shading normal and turn normal to front if necessary.
	Vec3f s_normal = ray.hit->getNormal(ray);
	if (dot(s_normal,ray.dir) > 0)
			s_normal = -s_normal;

    // (2) Calculate ambient term (intensity = 1).
	Vec3f rad_amb = Vec3f(1.0,1.0,1.0);
	Vec3f intensity_high = Vec3f(1.0,1.0,1.0);
	Vec3f ambient_term = ka * componentProduct(intensity_high, rad_amb);

    // (3) Start shadow ray from surface point and iterate over all light
    //     sources (diffuse and specular term).
	Vec3f diffuse_term = Vec3f (0.0, 0.0, 0.0);
	Vec3f specular_term = Vec3f (0.0, 0.0, 0.0);

	//compute shadow ray
	Ray shadow_ray = Ray();
	shadow_ray.org = ray.org + ray.dir * ray.t;

	//int num_lights = scene->lights.size();
	for (std::vector<Light*>::iterator l = scene->lights.begin(); l != scene->lights.end(); ++l)
    {
		// ERROR: komme nicht an die daten der lichtquellen (position und intensity)
		// Alex: Bitte schoen :>
		///////////////////////////////////////////////////////////////////////////
		
		//only calcualte albedo from all PointLights because there are no ohter light types yet
		/*if(typeid(*l) != typeid(PointLight*)) {
			continue;
		}*/ //it's better to work with a dynamic cast I think...
		Vec3f intensity;
		if((*l)->illuminate(shadow_ray, intensity) && 	!scene->isOccluded(shadow_ray)) {
		
			// You have to cast the general light class to its derivative "PointLight" because "Light" isn't holding neither an Intensity nor a position.
		// shadow ray dir to specific light source
		//shadow_ray.dir = ((PointLight*)(*l))->position - shadow_ray.org;
		//normalize(shadow_ray.dir);
		//add diffuse term for each light detected
		
		    diffuse_term +=  componentProduct(kd *color, intensity* max(dot(shadow_ray.dir, s_normal), 0)) ;
		//add diffuse term for each light detected
		//specular_term += ks * componentProduct(color, ((PointLight*)(*l))->intensity) * pow(dot(shadow_ray.dir, s_normal), ke);
		}
	}

    // (4) Abort if the shadow ray is occluded.


	result = ambient_term + diffuse_term + specular_term;
    // IMPLEMENT ME END

    return result;
  }

};

#endif
