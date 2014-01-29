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


#include "../include/Object.h"

void Object::computeBSphere(glm::vec3& center, float& radius) const {
	AABB bbox = computeAABB();
	center = bbox.center();
	radius = glm::length(bbox.diagonal())*0.5f;
}