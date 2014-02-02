#ifndef __SCENE_H__
#define __SCENE_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/
#include <iostream>

#include "Camera.h"
#include "Object.h"
#include "Acceleration.h"
#include "AABB.h"

class  Scene {
public:
	Scene() : mEnableShadows(false), mDOF(false), mAccelerated(false) {}

	//const Shader *getShader(const geometry::Ray &r) const;

	const std::vector<Light*> &getLights() const    { return mLights; }
	//const std::vector<Shader*> &getShaders()  const       { return mShaders; }
	const std::vector<Object*> &getObjects() const { return mObjects; }

	void addLight(Light* l)     { mLights.push_back(l); }
	//void addShader(Shader* s)         { mShaders.push_back(s); }
	void addObject(Object *o) { mObjects.push_back(o); }

	void setCamera(Camera *c) { mCamera = c; }

	bool intersect(Ray& r) const {
		if (mAccelerated)
			return mAccelTree->intersect(r);

		bool res = false;
		for(int i = 0; i < mObjects.size(); i++) {
			res |= mObjects[i]->intersect(r);
		}
		return res;
	}

	const Camera *getCamera() const { return mCamera; }

	bool shadows() { return mEnableShadows; }
    bool dof()     {  return mDOF; }
	void enableShadow(bool b)  { mEnableShadows = b; }
	void enableDOF(bool b)     { mDOF = b; }

	void buildAccelerationStructure() {
		calcBounds(mAABB);
		std::cout << "KdTree: Scene bounds are: " << std::endl;
		std::cout << "min: " << mAABB.p_min.x << " | "<< mAABB.p_min.y << " | " << mAABB.p_min.z << std::endl;
		std::cout << "max: " << mAABB.p_max.x << " | "<< mAABB.p_max.y << " | " << mAABB.p_max.z << std::endl;
		mAccelTree = new KdTree(mAABB, mObjects);
		std::cout << "KdTree built!" << std::endl;
		mAccelerated = true;
	}
private:
	//std::vector<Shader *>          mShaders;
	std::vector<Light*>  mLights;
	std::vector<Object*> mObjects;

	Camera *mCamera;

	//status flags
	bool    mEnableShadows;
	bool    mDOF;
	bool    mAccelerated;
	KdTree *mAccelTree;
	AABB    mAABB;

	void calcBounds(AABB &aabb) {
		mAABB.reset();
		for(int i = 0; i < mObjects.size(); i++) {
			AABB bb = mObjects[i]->computeAABB();
			aabb.addAABB(bb);
		}
	}
};


#endif