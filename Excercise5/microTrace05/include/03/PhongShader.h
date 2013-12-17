#pragma once
#ifndef PHONGSHADER_H
#define PHONGSHADER_H

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
    Vec3f result;

    // (1) Get shading normal and turn normal to front if necessary.
    // (2) Calculate ambient term (intensity = 1).
    // (3) Start shadow ray from surface point and iterate over all light
    //     sources (diffuse and specular term).
    // (4) Abort if the shadow ray is occluded.

    // Turn shading normal to front if necessary.
    Vec3f normal = ray.hit->getNormal(ray);
    if (dot(normal, ray.dir) > 0)
    {
      normal = -normal;
    }

    // Calculate ambient term (intensity = 1).
    Vec3f ambientIntensity(1, 1, 1);
    Vec3f ambientColor = ka * color;
    result = componentProduct(ambientColor, ambientIntensity);

    // Start shadow ray from surface point.
    Ray shadow;
    shadow.org = ray.org + ray.t * ray.dir;

    // Iterate over all light sources.
    for (unsigned int l = 0; l < scene->lights.size(); l++)
    {
      // Get direction to light, and intensity.
      Vec3f lightIntensity;
      if (scene->lights[l]->illuminate(shadow, lightIntensity))
      {
        // Path to light occluded? Then omit.
        if (scene->isOccluded(shadow)) continue;

        // Diffuse term.
        float cosLightNormal = dot(shadow.dir, normal);
        if (cosLightNormal > 0)
        {
          Vec3f diffuseColor = (kd * color) * cosLightNormal;
          result += componentProduct(diffuseColor, lightIntensity);
        }

        // Specular term (based on reflection vector).
        Vec3f reflect = ray.dir - 2 * dot(normal, ray.dir) * normal;
        float cosLightReflect = dot(shadow.dir, reflect);
        if (cosLightReflect > 0)
        {
          Vec3f specularColor = ks * Vec3f(1, 1, 1);  // white highlight
          specularColor *= powf(cosLightReflect, ke); // shininess factor
          result += componentProduct(specularColor, lightIntensity);
        }
      }
    }

    return result;
  }

};

#endif
