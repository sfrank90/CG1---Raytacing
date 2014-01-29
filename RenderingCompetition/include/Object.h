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
#include "Ray.h"
#include "AABB.h"
#include "Material.h"

class Object {
protected:
	std::shared_ptr<Material> mMaterial;
public:
	Object(std::shared_ptr<Material> m) : mMaterial(m) {}
	virtual ~Object() {}
	
	virtual void refine(std::vector<std::shared_ptr<Object> > &refined) const {
	}
	
	virtual bool intersect(Ray &ray) {
		return false;
	}
	virtual bool canIntersect() const {
		return true;
	}

	std::shared_ptr<Material> getMaterial() const { return mMaterial; }

	// Bounding Box issues
	virtual AABB computeAABB() const = 0;
	virtual void computeBSphere(glm::vec3& center, float& radius) const;
};

#endif