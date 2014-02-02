#ifndef __QUADLIGHT_H__
#define __QUADLIGHT_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/

//#include <glm/glm.hpp>
//
//class H4D_API PointLight : public LightSource {
//private:
//	glm::vec3 mPosition;  // origin
//    glm::vec3 mIntensity; // emission (red, green, blue)
//	float     mRadius;    //transform into a light bulb
//public:
//	PointLight(Scene *scene, glm::vec3 position, glm::vec3 intensity, float radius = 0.0f) : LightSource(scene), mPosition(position), mIntensity(intensity), mRadius(radius) {}
//	bool illuminate(geometry::Ray &lightray, glm::vec3 &intensity) const;
//	virtual bool emit_(geometry::Ray &ray, glm::vec3 &phi) const;
//};
//
//class H4D_API QuadAreaLight : public LightSource
//{
//private:
//  glm::vec3 power;
//  glm::vec3 p0,e1,e2;
//  float area;
//  glm::vec3 normal;
//public:
//  QuadAreaLight(Scene *scene,
//    glm::vec3 power,glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
//    : LightSource(scene),power(power),p0(p0),e1(p1-p0),e2(p3-p0)
//  { 
//    normal = glm::cross(e1,e2);
//    area =  glm::length(normal); 
//    normal = glm::normalize(normal);
//  };  
//
//  virtual bool illuminate(geometry::Ray &ray, glm::vec3 &intensity) const;
//  virtual bool emit_(geometry::Ray &ray, glm::vec3 &phi) const;
//
//  virtual glm::vec3 GetNormal(const glm::vec3 &position) const
//  {
//    return normal;
//  }
//
//
//};

#endif