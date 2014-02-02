#ifndef __OBJECT_H__
#define __OBJECT_H__

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
#include <memory> 
#include <vector>
#include "Common.h"

class Material;
class Light;
struct Ray;
struct AABB;
class Object {
protected:
	Material* mMaterial;
	bool mLight;
public:
	Object(Material* m, bool l=false) : mMaterial(m), mLight(l) {
	}
	virtual ~Object() {}
	
	virtual void refine(std::vector<Object*> &refined) const {
	}
	
	virtual bool intersect(Ray &ray) {
		return false;
	}
	virtual bool canIntersect() const {
		return true;
	}
	const Material *getMaterial() const { 
		return mMaterial; 
	}

	bool isLight() {
		return mLight;
	}


	// Bounding Box issues
	virtual AABB computeAABB() const = 0;
	virtual void computeBSphere(glm::vec3& center, float& radius) const;
};

#endif