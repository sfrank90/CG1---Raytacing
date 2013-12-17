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
    Vec3f result;

    // (1) Get shading normal and turn normal to front if necessary.
    // (2) Calculate ambient term (intensity = 0).
    // (3) Start shadow ray from surface point and iterate over all light
    //     sources, using phi and theta1/theta2.
    // (4) Do not forget to attenuate each light using theta2, because
    //     this attenuation is not included in the BRDF data.
    // (5) Abort light source if the shadow ray is occluded.

    // Turn shading normal to front if necessary.
    Vec3f normal = ray.hit->getNormal(ray);
    if (dot(normal, ray.dir) > 0)
    {
      normal = -normal;
    }

    // Calculate theta and phi.
    double theta_in = acos(dot(-normal, ray.dir));
    double phi_in = 0.0;

    // Calculate ambient term (intensity = 0).
    result = Vec3f(0);

    // Start shadow ray from surface point.
    Ray shadow;
    shadow.org = ray.org + ray.t * ray.dir;

    // Iterate over all light sources.
    for (unsigned int l = 0; l < scene->lights.size(); l++)
    {
      // Get direction to light, and intensity.
      Vec3f lightIntensity;
      for (unsigned int s = 0; s < this->num_samples; s++)
      {
        if (scene->lights[l]->illuminate(shadow, lightIntensity))
        {
          // Diffuse term.
          float cosLightNormal = dot(shadow.dir, normal);
          if (cosLightNormal > 0)
          {
            if (scene->isOccluded(shadow)) continue;
            double theta_out = acos(cosLightNormal);

            // Derive local coordinate system.
            Vec3f x = cross(-ray.dir, normal);
            Vec3f y = cross(normal, x);

            // Project outgoing vector into local coordinate system.
            Vec3f c = cross(shadow.dir, normal);
            double phi_out = atan2(dot(c, y), dot(c, x));

            // Calculate colors.
            double r, g, b;
            read->lookup_brdf_val(theta_in, phi_in, theta_out, phi_out, r, g, b);
            Vec3f diffuseColor((float) r, (float) g, (float) b);
            diffuseColor = componentProduct(scale, diffuseColor) * cosLightNormal;
            result += componentProduct(diffuseColor, lightIntensity);
          }
        }
      }
      result /= (float) this->num_samples;
    }

    return result;
  }

};

#endif
