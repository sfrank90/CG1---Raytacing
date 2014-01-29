#ifndef __ACCELERATION_H__
#define __ACCELERATION_H__

/*
* ****************************************
*
* Harmony4D - Praktische Aspekte der Informatik WS1314
*
* (c) 2013 by Alexander Knueppel         
*
* ****************************************
*
*/


#include <glm/glm.hpp>
#include "Object.h"
#include "AABB.h"

struct KdTreeNode {
	KdTreeNode (std::vector<std::shared_ptr<Object> > &objects) : mSplitPlane(0.0), mDimension(0), mLeftChild(nullptr), mRightChild(nullptr) {
		mObjects = objects;

	}

    ~KdTreeNode () { }
	std::vector<std::shared_ptr<Object> > mObjects;    
	std::uint32_t mDimension;
    float         mSplitPlane;    
	KdTreeNode   *mLeftChild, *mRightChild;
    //AABB          mBB;

	bool traverse(Ray &ray, float &t0, float &t1) {
		//leaf!!
	    if(mLeftChild == nullptr && mRightChild == nullptr) {
			  for (int i = 0; i < (int) mObjects.size(); i++)  
				  mObjects[i]->intersect(ray);
			  return ray.hitObject != nullptr && ray.t < t1;
		}

		float d = (mSplitPlane - ray.origin[mDimension]) / ray.dir[mDimension];

		// Determine front and back side.
		//int front = ray.dir[mDimension] < 0 ? 1 : 0;
		//int back = 1 - front;

		KdTreeNode *child1, *child2;
		if(ray.dir[mDimension] < 0) {
			child1 = mLeftChild;
			child2 = mRightChild;
		} else {
			child1 = mRightChild;
			child2 = mLeftChild;
		}

		if(d <= t0) {
			return child1->traverse(ray, t0, t1); 
		} else if (d >= t1) {
			return child2->traverse(ray, t0, t1); 
		} 
		if(child2->traverse(ray, t0, d)) 
			return true;
		  
		return child1->traverse(ray, d, t1); 
	}
};

class KdTree {
public:
        KdTree(AABB bb, std::vector<std::shared_ptr<Object> > objects, std::uint32_t maxObjects = 4, std::uint32_t maxDepth = 20) : mBB(bb), mMaxObjects(maxObjects), mMaxDepth(maxDepth) {
			mRoot = nullptr;
			mRoot = buildTree(mBB, objects, 0);
		}
		~KdTree() {
			if(mRoot)
				delete mRoot;
		}

        KdTreeNode* getRoot () const { return mRoot; }

		bool intersect(Ray &ray) {
			float t0 = 0;
			float t1 = ray.t;

			mBB.clipRay(ray, t0, t1);
			if (t1 - t0 < constants::epsilon)
			{
			  return false;
			}

			mRoot->traverse(ray, t0, t1);
			if (ray.hitObject != nullptr)
			  return true;

			return false;
		}
private:
	  KdTreeNode* buildTree (AABB bb, std::vector<std::shared_ptr<Object> > objects, std::uint32_t depth) {

		  if (depth > mMaxDepth || (int) objects.size() <= mMaxObjects) 
			  return new KdTreeNode(objects);


			KdTreeNode *node = new KdTreeNode(objects);

			glm::vec3 diam = bb.p_max - bb.p_min;
			
			std::uint32_t dim = diam.x >= diam.y ? 0 : 1;
			if(diam.z > diam[dim]) dim = 2;

			node->mDimension = dim;

			AABB lBounds = bb;
			AABB rBounds = bb;

			std::vector<std::shared_ptr<Object> > lPrim;
			std::vector<std::shared_ptr<Object> > rPrim;

			node->mSplitPlane = (bb.p_max[dim]+bb.p_min[dim]) / 2.0f;
	
			lBounds.p_max[dim] = node->mSplitPlane;
			rBounds.p_min[dim] = node->mSplitPlane;

			for(std::vector<std::shared_ptr<Object> >::iterator it = objects.begin(); it != objects.end(); ++it) 
			{
				// add left and right primitives to specific vector
				
				if((*it)->computeAABB().intersect(lBounds)) {
					lPrim.push_back(*it);
				}
				if((*it)->computeAABB().intersect(rBounds)) {
					rPrim.push_back(*it);
				}
			}

			node->mLeftChild = buildTree(lBounds, lPrim, depth + 1); 
			node->mRightChild = buildTree(rBounds, rPrim, depth + 1);

			return node;
	  }

      KdTreeNode*   mRoot;
	  AABB			mBB;
	  std::uint32_t mMaxObjects;
	  std::uint32_t mMaxDepth;
};

#endif
