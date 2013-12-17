#pragma once
#ifndef SCENE04_H
#define SCENE04_H
#define USE_EXAMPLE_OBJ_MODEL 0

#include "../Scene.h"
#include "../01/PerspectiveCamera.h"
#include "../02/PointLight.h"
#include "../03/FlatShader.h"
#include "../03/PhongShader.h"
#include "../03/BRDFShader.h"
#include "../03/Triangle.h"
#include "../03/Sphere.h"
#include "../03/InfinitePlane.h"
#include "AirBoxGenerator.h"
#include "RefractiveShader.h"
#include "SpotLight.h"

class Scene04: public Scene
{
public:

  Scene04(int resx, int resy)
      : Scene(resx, resy)
  {
  }

  virtual ~Scene04()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Builds the scene.
   */
  void build04(const double task)
  {
    this->camera = new PerspectiveCamera(Vec3f(0, 0.8, 11), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 60, resx, resy);

    // Load scene description.
    if (task >= 4.3 && task < 4.4)
    { // mirages.
      AirBoxGenerator ag(0.05, this, -1.0, 2.0);
      ag.generatePlanes();
      this->camera = new PerspectiveCamera(Vec3f(0, 0.2, 60), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 8, resx, resy);
    }

    // Phong/flat/BRDF shaders.
    PhongShader *shd1 = new PhongShader(this, Vec3f(1, 0, 0), 0.1, 0.5, 0.5, 40); // red surface
    PhongShader *shd2 = new PhongShader(this, Vec3f(0, 1, 0), 0.1, 0.5, 0.5, 40); // green surface
    PhongShader *shd3 = new PhongShader(this, Vec3f(0, 0, 1), 0.1, 0.5, 0.5, 40); // blue surface
    PhongShader *shd4 = new PhongShader(this, Vec3f(1, 1, 0), 0.1, 0.5, 0.5, 40); // yellow surface
    PhongShader *shd5 = new PhongShader(this, Vec3f(0, 1, 1), 0.1, 0.5, 0.5, 40); // cyan surface
    FlatShader *fshd = new FlatShader(this, Vec3f(1, 1, 0));
    BRDFShader *shdBRDF1 = new BRDFShader(this, "materials/brdfs/green-acrylic.binary", Vec3f(10, 10, 10));

    // Scene objects.
    Primitive *s1, *s2, *s3, *p1, *t1;

    // Scene objects (primitives).
    s1 = new Sphere(Vec3f(-2, 1.7, 0), 2, shd1);
    s3 = new Sphere(Vec3f(3, 0.8, -2), 2, shd3);
    if (task >= 4.1 && task < 4.2)
    { // refraction
      RefractiveShader *shdRef1 = new RefractiveShader(this, Vec3f(1.0, 1.0, 1.0), 1.0, 1.0, 1.3);
      p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), shd4);
      s2 = new Sphere(Vec3f(1, -1, 1), 2.2, shdRef1);
    }
    else if (task >= 4.3 && task < 4.4)
    { // mirages
      p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), fshd);
      s2 = new Sphere(Vec3f(1, -1, 1), 2.2, shd2);
    }
    else if (task >= 4.2 && task < 4.3)
    { // spotlight
      p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), shd4);
      s2 = new Sphere(Vec3f(1, -1, 1), 2.2, shdBRDF1);
    }
    else
    {
      p1 = s2 = NULL;
      cout << "Task unknown: " << task << "\n";
      return;
    }
    t1 = new Triangle(Vec3f(-2, 3, 1), Vec3f(1, 2, 1), Vec3f(3, 2.8, 3), shd5);

    // Add primitives to scene.
    this->add(s1);
    this->add(s2);
    this->add(s3);
    this->add(p1);
    this->add(t1);

    // Add lights.
    Vec3f pointLightIntensity(50);
    Vec3f lightPosition1(4, 5, 6);
    Vec3f lightPosition2(-3, 5, +4);
    if (task >= 4.2 && task < 4.3)
    { // spot light
      Vec3f spotLightSourceIntensity1(150, 150, 150);
      Vec3f spotLightSourceIntensity2(50, 50, 50);
      Vec3f lightDir1 = Vec3f(1, -1, 1) - lightPosition1;
      Vec3f lightDir2 = Vec3f(-2, -1, 2) - lightPosition2;
      normalize(lightDir1);
      normalize(lightDir2);
      float alpha_min1 = 15.0f, alpha_max1 = 30.0f;
      float alpha_min2 = 5.0f, alpha_max2 = 10.0f;
      SpotLight *spotLight1 = new SpotLight(this, lightPosition1, lightDir1, spotLightSourceIntensity1, alpha_min1, alpha_max1);
      SpotLight *spotLight2 = new SpotLight(this, lightPosition2, lightDir2, spotLightSourceIntensity2, alpha_min2, alpha_max2);
      this->add(spotLight1);
      this->add(spotLight2);
    }
    else
    {
      PointLight *pointLight1 = new PointLight(this, lightPosition1, pointLightIntensity);
      PointLight *pointLight2 = new PointLight(this, lightPosition2, pointLightIntensity);
      this->add(pointLight1);
      this->add(pointLight2);
    }
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
