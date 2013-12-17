#pragma once
#ifndef KDTREE_H
#define KDTREE_H

#include "Box.h"
#include "Primitive05.h"

struct KDTree
{

  // --------------------------------------------------------------------------

  struct KDNode
  {
    virtual bool traverse(Ray &ray, float &t0, float &t1) = 0;
  };

  // --------------------------------------------------------------------------

  struct KDInnerNode: public KDNode
  {
    KDNode *child[2];
    float split;
    int dim;

    KDInnerNode()
        : split(0.0), dim(0)
    {
    }

    virtual ~KDInnerNode()
    {
    }

    virtual bool traverse(Ray &ray, float &t0, float &t1)
    {
      float d = (split - ray.org[dim]) / ray.dir[dim];

      // Determine front and back side.
      int front = ray.dir[dim] < 0 ? 1 : 0;
      int back = 1 - front;

      // IMPLEMENT ME 5.1c
      // Traverse tree and return child or true respectively.
      // IMPLEMENT ME END
    }

  };

  // --------------------------------------------------------------------------

  struct KDLeafNode: public KDNode
  {
    vector<Primitive *> primitive;

    KDLeafNode(vector<Primitive *> &prim)
    {
      primitive = prim;
    }

    virtual ~KDLeafNode()
    {
    }

    virtual bool traverse(Ray &ray, float &t0, float &t1)
    {
      for (int i = 0; i < (int) primitive.size(); i++)
      {
        primitive[i]->findIntersection(ray);
      }
      return ray.hit != NULL && ray.t < t1;
    }

  };

  // --------------------------------------------------------------------------

  Box bounds;
  int maxDepth;
  int minTri;

  // --------------------------------------------------------------------------

  KDNode *buildTree(Box &bounds, vector<Primitive *> prim, int depth)
  {
    // Build the kd-tree.
    // (1) Check maximum depth and minimum number of primitives.
    // (2) Create new KDLeafNode if one condition is true.
    if (depth > maxDepth || (int) prim.size() <= minTri)
    {
      return new KDLeafNode(prim);
    }

    KDInnerNode *node = new KDInnerNode;

    Vec3f diam = bounds.max - bounds.min;
    int dim = diam.maxDim();
    node->dim = dim;

    Box lBounds = bounds;
    Box rBounds = bounds;

    vector<Primitive *> lPrim;
    vector<Primitive *> rPrim;

    // IMPLEMENT ME 5.1c
    // (1) Split node.
    // (2) Iterate over primitives. Add left and right primitives.
    // (3) Build next trees using left and right bounds.
    // IMPLEMENT ME END

    return node;
  }

  // --------------------------------------------------------------------------

  KDNode *root;

  // --------------------------------------------------------------------------

  KDTree(Box &bounds, vector<Primitive *> kdTree)
      : bounds(bounds)
  {
    maxDepth = 20;
    minTri = 3;
    root = NULL;
    root = buildTree(bounds, kdTree, 0);
  }

  bool findIntersection(Ray &ray)
  {
    float t0 = 0;
    float t1 = ray.t;

    bounds.clipBox(ray, t0, t1);
    if (t1 - t0 < EPSILON)
    {
      return false;
    }

    root->traverse(ray, t0, t1);
    if (ray.hit != NULL)
    {
      return true;
    }

    return false;
  }

};

#endif
