#ifndef __RAY_H__
#define __RAY_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*
* TODO: Hit object and hit material...
*
*/

#include "Common.h"
#include <glm/glm.hpp>

struct Ray {

  // Constructor
  Ray() {  reset(); }

  Ray(const glm::vec3& _origin, 
	  const glm::vec3& _dir, 
	  float min_dist = constants::epsilon, 
	  float max_dist = constants::infinity, 
	  float _t = constants::epsilon, 
	  int depth = 0) 
      : origin(_origin), dir(_dir), hasHit(false), isInside(false), hitDiffuse(false), ior(1.0f), tmin(min_dist), t(_t), tmax(max_dist), recursiveDepth(depth) 
  { }

  //characteristics
  glm::vec3 origin;
  glm::vec3 dir;
  glm::vec3 hitPos;
  glm::vec3 hitNormal;

  //status
  bool hasHit;           // Did the ray hit an object
  bool isInside;         // Is the ray inside an object
  bool hitDiffuse;       // Did the path involve a diffuse object

  //parameter
  float t;               // Distance from origin to current intersection
  float ior;             // Current index of refraction for media
  float u, v;            // uv-coordinates on current surface
  float tmin, tmax;      // Near and far cut-off planes

  int recursiveDepth;       // Current recursion depth (path length)

  //TODO
  //const Shape* hitObject;

  //const Material* getHitMaterial() const {
  //  return nullptr;
  //}

  void reset() {
    hasHit = false;
    isInside = false;
    hitDiffuse = false;
    ior = 1.0f;
	tmin = constants::epsilon;
	tmax = constants::infinity;
    recursiveDepth = 0;
  }
};

#endif // RAY_H