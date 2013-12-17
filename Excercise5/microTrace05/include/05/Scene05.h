#pragma once
#ifndef SCENE05_H
#define SCENE05_H

#include "../Scene.h"
#include "../01/PerspectiveCamera.h"
#include "../02/PointLight.h"
#include "../03/FlatShader.h"
#include "../03/PhongShader.h"
#include "../03/BRDFShader.h"
#include "Sphere.h"
#include "InfinitePlane.h"
#include "Triangle.h"
#include "SmoothTriangle.h"
#include "KDTree.h"

class Scene05: public Scene
{

public:

  bool accelerated;
  KDTree* kdTree;
  Box boundingBox;

  Scene05(int resx, int resy)
      : Scene(resx, resy), accelerated(false), kdTree(NULL)
  {
  }

  virtual ~Scene05()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Builds the scene.
   */
  void build05(const double task)
  {
    this->camera = new PerspectiveCamera(Vec3f(0, -3, 10), Vec3f(0, 1, -1), Vec3f(0, 1, 0), 60, resx, resy);

    // Add lights.
    PointLight *pl1 = new PointLight(this, Vec3f(4, 5, 6), Vec3f(150));
    PointLight *pl2 = new PointLight(this, Vec3f(-4, 5, 6), Vec3f(100));
    this->add(pl1);
    this->add(pl2);

    // Load teapot in stadium.
    BRDFShader *bshd1 = new BRDFShader(this, "materials/brdfs/green-acrylic.binary", Vec3f(10));
    BRDFShader *bshd2 = new BRDFShader(this, "materials/brdfs/gold-paint.binary", Vec3f(10));
    parseOBJ("materials/models/teapot.obj", bshd1, NORMALS, SMOOTH, Vec3f(75), Vec3f(0));
    parseOBJ("materials/models/stadium.obj", bshd2, NORMALS, SMOOTH, Vec3f(75), Vec3f(0));

#if ENABLE_KDTREE == true
    // Build kd-tree.
    buildAccelerationStructure();
#endif
  }

  // --------------------------------------------------------------------------

  /**
   * Advances the camera and object state by one.
   */
  void tick(int t)
  {
    PerspectiveCamera *cam = (PerspectiveCamera*) this->camera;
    cam->initAxes();
  }

  // --------------------------------------------------------------------------

  /**
   * Creates a triangle (standard, smooth or textured).
   * - tri: coords.
   * - norm: normals.
   * - tex: texture coords.
   */
  virtual Primitive* createTrianglePrimitive(Shader* shd, TriangleStyle tstyle, OBJStyle objstyle, // shader, triangle style, object style.
      Vec3f* tri, Vec3f* norm, Vec3f* tex)
  {
    if (tstyle == STANDARD)
    {
      return new Triangle(tri[0], tri[1], tri[2], shd);
    }
    else if (tstyle == SMOOTH)
    {
      return new SmoothTriangle(tri[0], tri[1], tri[2], norm[0], norm[1], norm[2], shd);
    }
    else
    {
      printf("(Scene): TriangleStyle not supported. Exiting.\n");
      exit(1);
      return NULL;
    }
  }

  // --------------------------------------------------------------------------

  /**
   * Find intersections to any contained object.
   */
  virtual bool findIntersection(Ray &ray)
  {
    if (accelerated)
    {
      return kdTree->findIntersection(ray);
    }
    else
    {
      return Scene::findIntersection(ray);
    }
  }

  /**
   * Find occluder.
   */
  virtual bool isOccluded(Ray &ray)
  {
    if (accelerated)
    {
      return kdTree->findIntersection(ray);
    }
    else
    {
      return Scene::isOccluded(ray);
    }
  }

  // --------------------------------------------------------------------------

  /**
   * Calculates bounds of the scene.
   */
  void calcBounds(Box *boundingBox)
  {
    // IMPLEMENT ME 5.1b
    // (1) Reset the given bounding box.
    // (2) Get a box for each primitive, calculate their bounds.
    // (3) Extend the values of the empty bounding box to include
    //     the box of each primitive.
    // IMPLEMENT ME END
  }

  /**
   * Builds a KD tree as acceleration structure.
   */
  void buildAccelerationStructure()
  {
    calcBounds(&boundingBox);
    cout << "KdTree: Scene bounds are: " << boundingBox.min << " to " << boundingBox.max << endl;
    kdTree = new KDTree(boundingBox, primitive);
    accelerated = true;
  }

};

#endif
