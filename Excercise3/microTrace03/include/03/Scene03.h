#pragma once
#ifndef SCENE03_H
#define SCENE03_H
#define USE_EXAMPLE_OBJ_MODEL 0

#include "../Scene.h"
#include "../Shader.h"
#include "../01/PerspectiveCamera.h"
#include "../02/PointLight.h"
#include "Sphere.h"
#include "InfinitePlane.h"
#include "Triangle.h"
#include "BRDFShader.h"
#include "PhongShader.h"
#include "EyeLightShader.h"
#include "MirrorShader.h"

class Scene03: public Scene
{
public:

  Scene03(int resx, int resy)
      : Scene(resx, resy)
  {
  }

  virtual ~Scene03()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Builds the scene.
   */
  void build03(const double task)
  {
    this->camera = new PerspectiveCamera(Vec3f(0, 0, 8), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 60, RESX, RESY);

    // Default: EyeLight surface shaders.
    EyeLightShader *eshd1 = new EyeLightShader(this, Vec3f(1, 0, 0)); // red surface
    EyeLightShader *eshd2 = new EyeLightShader(this, Vec3f(0, 1, 0)); // green surface
    EyeLightShader *eshd3 = new EyeLightShader(this, Vec3f(0, 0, 1)); // blue surface
    EyeLightShader *eshd4 = new EyeLightShader(this, Vec3f(1, 1, 0)); // yellow surface
    EyeLightShader *eshd5 = new EyeLightShader(this, Vec3f(0, 1, 1)); // cyan surface

    // Scene objects.
    Primitive *s1, *s2, *s3, *p1, *t1;
    s1 = new Sphere(Vec3f(-2, 1.7, 0), 2, eshd1);
    s2 = new Sphere(Vec3f(1, -1, 1), 2.2, eshd2);
    s3 = new Sphere(Vec3f(3, 0.8, -2), 2, eshd3);
    p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), eshd4);
    t1 = new Triangle(Vec3f(-2, 3, 1), Vec3f(1, 2, 1), Vec3f(3, 2.8, 3), eshd5);

    // Mirror Shader 1.
    if (task == 3.11)
    {
      cout << "Task: " << "Mirror 1 " << task << "\n";
      MirrorShader *mshd1 = new MirrorShader(this, Vec3f(1, 1, 1));
      p1->shader = mshd1;
    }

    // Mirror Shader 2.
    else if (task == 3.12)
    {
      cout << "Task: " << "Mirror 2 " << task << "\n";
      MirrorShader *mshd2 = new MirrorShader(this, Vec3f(1, 1, 1));
      s2->shader = mshd2;
    }

    // Phong.
    else if (task >= 3.2 && task < 3.3)
    {
      // Phong surface shaders.
      cout << "Task: " << "Phong " << task << "\n";
      PhongShader *pshd1 = new PhongShader(this, Vec3f(1, 0, 0), 0.1, 0.5, 0.5, 40); // red surface
      PhongShader *pshd2 = new PhongShader(this, Vec3f(0, 1, 0), 0.1, 0.5, 0.5, 40); // green surface
      PhongShader *pshd3 = new PhongShader(this, Vec3f(0, 0, 1), 0.1, 0.5, 0.5, 40); // blue surface
      PhongShader *pshd4 = new PhongShader(this, Vec3f(1, 1, 0), 0.1, 0.5, 0.5, 40); // yellow surface
      PhongShader *pshd5 = new PhongShader(this, Vec3f(0, 1, 1), 0.1, 0.5, 0.5, 40); // cyan surface
      s1->shader = pshd1;
      s2->shader = pshd2;
      s3->shader = pshd3;
      p1->shader = pshd4;
      t1->shader = pshd5;
    }

    // BRDF Shaders.
    else if (task == 3.3)
    {
      cout << "Task: " << "BRDF " << task << "\n";
      BRDFShader *shdBRDF1 = new BRDFShader(this, "materials/brdfs/green-acrylic.binary", Vec3f(10, 10, 10));
      BRDFShader *shdBRDF2 = new BRDFShader(this, "materials/brdfs/gold-paint.binary", Vec3f(5, 5, 5));
      BRDFShader *shdBRDF3 = new BRDFShader(this, "materials/brdfs/grease-covered-steel.binary", Vec3f(15, 15, 15));
      BRDFShader *shdBRDF4 = new BRDFShader(this, "materials/brdfs/pvc.binary", Vec3f(20, 20, 20));
      BRDFShader *shdBRDF5 = new BRDFShader(this, "materials/brdfs/color-changing-paint3.binary", Vec3f(5000, 5000, 5000));
      s1->shader = shdBRDF1;
      s2->shader = shdBRDF2;
      s3->shader = shdBRDF3;
      p1->shader = shdBRDF4;
      t1->shader = shdBRDF5;
    }

    // Undefined case.
    else
    {
      cout << "Task unknown: " << task << "\n";
      return;
    }

    // Add primitives to scene.
    this->add(s1);
    this->add(s2);
    this->add(s3);
    this->add(p1);
    this->add(t1);

    // Light sources (2 point lights).
    Vec3f pointLightIntensity(50, 50, 50);
    Vec3f lightPosition1(+4, +5, +6);
    Vec3f lightPosition2(-3, +5, 4);
    PointLight *pointLight1 = new PointLight(this, lightPosition1, pointLightIntensity);
    PointLight *pointLight2 = new PointLight(this, lightPosition2, pointLightIntensity);
    this->add(pointLight1);
    this->add(pointLight2);

    // Add OBJ model.
#if USE_EXAMPLE_OBJ_MODEL > 0
    parseOBJ("materials/models/cube.obj", eshd5, NONORMALS, STANDARD, Vec3f(0.1f, 0.05f, 0.15f), Vec3f(-1, -.5, 4));
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
   * Creates a basic (untextured, non-smooth) triangle.
   */
  virtual Primitive* createTrianglePrimitive(Shader* shd, TriangleStyle tstyle, OBJStyle objstyle, Vec3f* tri, Vec3f* norm, Vec3f* tex)
  {
    return new Triangle(tri[0], tri[1], tri[2], shd);
  }

};

#endif
