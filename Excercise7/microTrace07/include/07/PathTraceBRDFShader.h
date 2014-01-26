#pragma once
#ifndef PATHTRACEBRDFSHADER_H
#define PATHTRACEBRDFSHADER_H

#include "../Shader.h"
#include "../03/BRDFRead.h"

class PathTraceBRDFShader: public Shader
{

private:
  BRDFRead *read;
  Vec3f scale;

public:

  PathTraceBRDFShader(Scene *scene, const char* file, Vec3f scale)
      : Shader(scene), scale(scale)
  {
    read = new BRDFRead(file);
  }

  virtual ~PathTraceBRDFShader()
  {
  }

  virtual string getClassName() const
  {
    return string("pathtrace_brdf09");
  }

  virtual bool isTransparent()
  {
    return false;
  }

  virtual Vec3f shadeRay(Ray &ray)
  {
    // Get shading normal.
    Vec3f normal = ray.hit->getNormal(ray);
    normalize(normal);

    // Turn normal to front if necessary.
    if (dot(normal, ray.dir) > 0)
    {
      normal = -normal;
    }

    // Calculate theta and phi.
    double theta_in = acos(dot(-normal, ray.dir));
    double phi_in = 0.0;

    // Start shadow ray from surface point.
    Ray shadow;
    shadow.org = ray.org + ray.t * ray.dir;

    // Direct illumination: Iterate over all light sources.
    Vec3f result_direct(0.0, 0.0, 0.0);
    for (unsigned int l = 0; l < scene->lights.size(); l++)
    {
      // Get direction to light, and intensity.
      Vec3f lightIntensity;
      for (unsigned int s = 0; s < this->num_samples; s++)
      {
        if (scene->lights[l]->illuminate(shadow, lightIntensity))
        {
          float cosLightNormal = dot(shadow.dir, normal);
          if (cosLightNormal > 0)
          {
            if (scene->isOccluded(shadow)) continue;
            double theta_out = acos(dot(normal, shadow.dir));
            // Calculate phi_out, the difference between the projections of
            // the incoming and outgoing ray.
            Vec3f ortho1 = cross(-ray.dir, normal);

            // Derive local coordinate system.
            Vec3f coordX = ortho1;
            Vec3f coordY = cross(normal, coordX);

            // Project outgoing vector into local coordinate system.
            Vec3f ortho2 = cross(shadow.dir, normal);
            double phi_out = atan2(dot(ortho2, coordY), dot(ortho2, coordX));

            // Calculate colors.
            double red_val, green_val, blue_val;
            read->lookup_brdf_val(theta_out, phi_out, theta_in, phi_in, red_val, green_val, blue_val);
            Vec3f diffuseColor((float) red_val, (float) green_val, (float) blue_val);
            result_direct += componentProduct(componentProduct(scale, diffuseColor) * cosLightNormal, lightIntensity);
          }
        }
      }
      result_direct /= (float) this->num_samples;
    }

    // Indirect lighting (unless recursion is too deep already).
    if (ray.recursionLevel >= this->num_bounces)
    {
      return result_direct;
    }
    const float pdf = 1.f / (2.f * M_PI); // pdf for uniform sampling
    Vec3f result_indirect(0.0, 0.0, 0.0);

    // IMPLEMENT ME 7.1
    // (1) Choose uniformly random angles phi_out and theta_out on the
    //     hemisphere around the normal.
    // (2) Translate that into a ray in the local coordinate system,
    //     which has the normal as y-axis.
    // (3) Translate the ray into the global coordinate system.
    // (4) Cast that ray and add it (weighted by cosine to normal, and by
    //     BRDF absorption/reflection rate; also do not forget the PDF)
    //     to the result.
    // IMPLEMENT ME END

    float u1  = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float u2  = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    //Vec3f v;
    const float theta = acos(sqrt(1.0f - u1));
    const float phi = 2.0f * M_PI * u2;
    // Calculate new direction as if the y-axis were the normal
    //Vec3f dir(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

    // du hast y und z vertauscht?!
    // in der folie steht folgende konvertierung (slide 8, page 42)
	// -> ja du hast recht, duerfte aber keinen unterschied machen...
    
	//dir as y-axis
	Vec3f dir(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));

 //   const float a = 1.0f/(1.0f + normal.z);
 //   const float b = -normal.x*normal.x*a;

 //   // hier überschreibst du den oben erstellten vector "dir" wieder?!
	//// ja, benutze aber dir auch. Ich muss die "Hemisphere" ja jetzt drehen...
 //   dir =   Vec3f(1.0f - normal.x*normal.x*a, b, -normal.x)*dir.x
 //       +   Vec3f(b, 1.0f - normal.y*normal.y*a, -normal.y)*dir.y 
 //       +   normal*dir.z;

	//Ich mach's anders,... statt Drehmatrix lieber mit einem KS

	//create coordiante system
	Vec3f up = dir;
	if(fabs(up.x) <= fabs(up.y) && fabs(up.x) <= fabs(up.z)) {
		up.x = 1.0f;
	} else if(fabs(up.y) <= fabs(up.x) && fabs(up.y) <= fabs(up.z)) {
		up.y = 1.0f;
	} else {
		up.z = 1.0f;
	}

	Vec3f x = cross(up,normal); normalize(x);
	Vec3f z = cross(x, normal); normalize(z);

	//normal is y-axis
	dir = dir.x * x + dir.y * normal + dir.z * z;
	normalize(dir);

	Ray newRay(ray.org + ray.t*ray.dir, dir);
	newRay.recursionLevel = ray.recursionLevel+1;

	float cw = dot(newRay.dir, normal);
	result_indirect += (cw * scale * scene->rayTrace(newRay)) * pdf;

    return result_direct + result_indirect;
  }

};

#endif
