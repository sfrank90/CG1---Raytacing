#ifndef __LIGHT_H__
#define __LIGHT_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/

#include <glm/glm.hpp>
#include "Ray.h"
#include "Sampling.h"
#include "Scene.h"
class Light {
protected:
	Scene* mScene;
public:
	Light(Scene *s)  : mScene(s) {}
	//virtual float emit(Ray &ray, float wavelength) const { 
	//	return false; 
	//}
	virtual bool illuminate(Ray &lightray, glm::vec3 &intensity) const = 0;
	virtual bool emit(Ray &ray, glm::vec3 &phi) const = 0;
};

class PointLight : public Light{
private:
	glm::vec3 mPosition;  // origin
    glm::vec3 mIntensity; // emission (red, green, blue)
	float     mRadius;    //transform into a light bulb
public:
	PointLight(Scene *scene, glm::vec3 position, glm::vec3 intensity, float radius = 0.0f) : Light(scene), mPosition(position), mIntensity(intensity), mRadius(radius) {}
	bool illuminate(Ray &lightray, glm::vec3 &intensity) const {
		if(mRadius > 0.0f) { //this light has a real area
			Sampling smp;
			glm::vec3 newPos = mRadius*smp.sampleSphere()+mPosition;
			lightray.dir = newPos - lightray.origin;
		} else
			lightray.dir = mPosition - lightray.origin;

		lightray.hitObject = nullptr;
		lightray.t = glm::length(lightray.dir) - constants::epsilon;
		lightray.dir = glm::normalize(lightray.dir);

		float sqDist = lightray.t * lightray.t;
		float attenuation = 1.0 / sqDist; // due to point light.
		
		float area = 1.f;
		if(mRadius > 0.0f)
			area = 4.f * constants::pi * mRadius*mRadius;
		
		intensity = area* attenuation * mIntensity;

		if(mScene->shadows()) {
			Ray occluder = lightray;
			occluder.origin += (float)(constants::epsilon)*occluder.dir;
			if(mScene->intersect(occluder) && !occluder.hitObject->isLight()) {
				//in shadow
				return false;
			}
		}
		return true;
	}
	bool emit(Ray &ray, glm::vec3 &phi) const {
		Sampling smp;
		ray.dir = smp.sampleSphere();
		ray.origin = mPosition + ray.dir * mRadius;
		if (mScene->intersect(ray)) {
			phi = mIntensity * (float)constants::sqrd_pi*4.f*mRadius*mRadius;
			return true;
		}
		return false;
	}
};

class QuadAreaLight : public Light {
private:
  glm::vec3 power;
  glm::vec3 p0,e1,e2;
  float area;
  glm::vec3 normal;
public:
  QuadAreaLight(Scene *scene,
    glm::vec3 power,glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
    : Light(scene),power(power),p0(p0),e1(p1-p0),e2(p3-p0)
  { 
    normal = glm::cross(e1,e2);
    area =  glm::length(normal); 
    normal = glm::normalize(normal);
  };  

  virtual bool illuminate(Ray &ray, glm::vec3 &intensity) const {
	  RandomFloat rnd;
    float xi1 = rnd();
    float xi2 = rnd();

    glm::vec3 pos = p0 + xi1 * e1 + xi2 * e2;

    ray.dir = pos - ray.origin;

    float dist = glm::length(ray.dir);

    float cosN = - glm::dot(ray.dir,normal) / dist;

    if (cosN <= 0.0f) 
      return false;

    intensity = (area * cosN / (dist * dist) ) * power;

    ray.dir = glm::normalize(ray.dir);

	ray.t = dist - constants::epsilon;
	ray.hitObject  = nullptr;
	bool inShadow = false;
	if(mScene->shadows()) {
		if(mScene->intersect(ray) && !ray.hitObject->isLight()) {
			//in shadow
			return false;
		}
	}
	return true;
  }
  virtual bool emit(Ray &ray, glm::vec3 &phi) const {
    RandomFloat rnd;
    float xi1 = rnd();
    float xi2 = rnd();

	Sampling smp;
	glm::vec3 pos = p0 + xi1 * e1 + xi2 * e2;
	ray.origin = pos;
	ray.dir = smp.sampleCW(normal);

	if (mScene->intersect(ray)) {
		phi = power * (float)constants::pi * area;
		return true;
	}
	return false;
  }

  virtual glm::vec3 GetNormal(const glm::vec3 &position) const
  {
    return normal;
  }


};

#endif