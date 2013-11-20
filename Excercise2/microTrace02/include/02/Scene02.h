#pragma once
#ifndef SCENE02_H
#define SCENE02_H

#include "../Scene.h"
#include "../01/PerspectiveCamera.h"
#include "Sphere.h"
#include "Triangle.h"
#include "InfinitePlane.h"
#include "PointLight.h"

class Scene02: public Scene
{

public:

  Scene02(int resx, int resy)
      : Scene(resx, resy)
  {
  }

  virtual ~Scene02()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Builds the scene.
   */
  void build02(const double task)
  {
    this->camera = new PerspectiveCamera(Vec3f(0, 0, 8), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 60, RESX, RESY);

    // Light sources.
    Vec3f pointLightIntensity(30, 30, 30);
    Vec3f lightPosition1(4, 3, 6);
    PointLight* pointLight1 = new PointLight(this, lightPosition1, pointLightIntensity);

    // Load scene description.
    Sphere* s1 = new Sphere(Vec3f(-2, 1.7, 0), 2, pointLight1, Vec3f(1, 0, 0));
    Sphere* s2 = new Sphere(Vec3f(1, -1, 1), 2.2, pointLight1, Vec3f(0, 1, 0));
    Sphere* s3 = new Sphere(Vec3f(3, 0.8, -2), 2, pointLight1, Vec3f(0, 0, 1));
    InfinitePlane* p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), pointLight1, Vec3f(1, 1, 0));
    Triangle* t1 = new Triangle(Vec3f(-2, 3, 1), Vec3f(1, 2, 1), Vec3f(3, 2.8, 3), pointLight1, Vec3f(0, 1, 1));

    // Add primitives to scene.
    add(s1);
    add(s2);
    add(s3);
    add(p1);
    add(t1);
    add(pointLight1);
  }

  // --------------------------------------------------------------------------

  /**
   * Trace the given ray and shade it; return the color of the shaded ray.
   */
  Vec3f rayTrace2(Ray &ray)
  {
    // Check if the ray intersects an object in the scene.
    return findIntersection(ray) ? ((Primitive02*) ray.hit)->TraceLightRay(ray) : bgColor;
  }

  // --------------------------------------------------------------------------

  /**
   * Advances the camera and object state by one.
   */
  void tick(int t)
  {
    PerspectiveCamera *cam = (PerspectiveCamera*) this->camera;
    Sphere *s1 = (Sphere*) this->primitive[0];
    Sphere *s2 = (Sphere*) this->primitive[1];

    // IMPLEMENT ME 2.3
    // Make both camera position/direction, and some objects move.
    // IMPLEMENT ME END

    cam->initAxes();
  }

  // --------------------------------------------------------------------------

  /**
   * Ignore for now. We do not have shaders yet.
   */
  virtual Primitive* createTrianglePrimitive(Shader* shd, TriangleStyle tstyle, OBJStyle objstyle, Vec3f* tri, Vec3f* norm, Vec3f* tex)
  {
    return NULL;
  }

};

#endif
