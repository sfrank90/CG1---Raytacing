#ifndef __TRIANGLEMESH_H__
#define __TRIANGLEMESH_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*
*/

#include "../Common.h"
#include "../Object.h"
#include "Triangle.h"
#include "../AABB.h"
#include "../model_obj.h"
#include "../Material.h"
class TriangleMesh : public Object {
public:
	TriangleMesh(Material *mat = nullptr) : Object(mat) {}
	
	//bool load(const std::string &file) {

	//}
	void setData(std::shared_ptr<ModelOBJ> obj) {
		mObj = obj;
	}
	void refine(std::vector<Object* > &refined, glm::mat4 transform = glm::mat4(1.f)) const {
	//For everey mesh
	glm::mat4 tinvtransform = glm::transpose(glm::inverse(transform)); //normal correction
	AABB aabb;
	for (int i = 0; i < mObj->getNumberOfMeshes(); ++i) {
		ModelOBJ::Mesh m = mObj->getMesh(i);
		Material *material;
		if(mMaterial == nullptr) { //if no material specified, create one from .mat-file or default-values
			material = new Material;
			material->mAmbient.r = mObj->getMaterial(i).ambient[0]; 
			material->mAmbient.g = mObj->getMaterial(i).ambient[1]; 
			material->mAmbient.b = mObj->getMaterial(i).ambient[2]; 
			material->mDiffuse.r = mObj->getMaterial(i).diffuse[0]; 
			material->mDiffuse.g = mObj->getMaterial(i).diffuse[1]; 
			material->mDiffuse.b = mObj->getMaterial(i).diffuse[2]; 
			material->mSpecular.r = mObj->getMaterial(i).specular[0];
			material->mSpecular.g = mObj->getMaterial(i).specular[1];
			material->mSpecular.b = mObj->getMaterial(i).specular[2];
			material->ior = mObj->getMaterial(i).refraction;
			material->shininess = mObj->getMaterial(i).shininess;
			material->mTransmission = glm::vec3(1.f) - glm::vec3(mObj->getMaterial(i).alpha);
		} else
			material = mMaterial;
		for(int j = 0; j < m.triangleCount; ++j) {
			int indexA = mObj->getIndexBuffer()[m.startIndex + j*3 + 0];
			int indexB = mObj->getIndexBuffer()[m.startIndex + j*3 + 1];
			int indexC = mObj->getIndexBuffer()[m.startIndex + j*3 + 2];
			//get vertices
			ModelOBJ::Vertex a = mObj->getVertex(indexA), 
				             b = mObj->getVertex(indexB), 
							 c = mObj->getVertex(indexC);

			glm::vec3 vA = glm::vec3(transform*glm::vec4(a.position[0],a.position[1],a.position[2],1.0f)),
				      vB = glm::vec3(transform*glm::vec4(b.position[0],b.position[1],b.position[2],1.0f)),
					  vC = glm::vec3(transform*glm::vec4(c.position[0],c.position[1],c.position[2],1.0f)),
					  nA = glm::normalize(glm::vec3(tinvtransform*glm::vec4(a.normal[0],a.normal[1],a.normal[2],0.0f))),
					  nB = glm::normalize(glm::vec3(tinvtransform*glm::vec4(b.normal[0],b.normal[1],b.normal[2],0.0f))),
					  nC = glm::normalize(glm::vec3(tinvtransform*glm::vec4(c.normal[0],c.normal[1],c.normal[2],0.0f)));

			//create intersectable triangle
			refined.push_back(new Triangle(vA,  vB,  vC, nA, nB, nC, material));
		}

	}

	}
	bool canIntersect() const { return false; }

	AABB computeAABB() const {
		AABB bb;
		float x,y,z;
		mObj->getCenter(x,y,z);
		bb.addPoint(glm::vec3(x,y,z) - glm::vec3(mObj->getWidth()*0.5, mObj->getHeight()*0.5, mObj->getLength()*0.5));
		bb.addPoint(glm::vec3(x,y,z) + glm::vec3(mObj->getWidth()*0.5, mObj->getHeight()*0.5, mObj->getLength()*0.5));
		return bb;		  
	}


private:
	std::shared_ptr<ModelOBJ> mObj;
};


#endif