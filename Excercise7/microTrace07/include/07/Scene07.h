#pragma once
#ifndef SCENE07_H
#define SCENE07_H

#include "../05/Scene05.h"
#include "../01/PerspectiveCamera.h"
#include "../02/PointLight.h"
#include "../04/SpotLight.h"
#include "PathTraceBRDFShader.h"

class Scene07: public Scene05
{
public:

  Scene07(int resx, int resy)
      : Scene05(resx, resy)
  {
  }

  virtual ~Scene07()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Builds the scene.
   */
  void build07(const double task, int num_bounces)
  {
    this->camera = new PerspectiveCamera(Vec3f(-5, 14, 4), Vec3f(0, -1.5, -1), Vec3f(0, 1, 0), 60, RESX, RESY);

    // BRDF shaders.
    PathTraceBRDFShader *shdBRDF1 = new PathTraceBRDFShader(this, "materials/brdfs/green-acrylic.binary", Vec3f(1.f)); // 10
    PathTraceBRDFShader *shdBRDF2 = new PathTraceBRDFShader(this, "materials/brdfs/gold-paint.binary", Vec3f(1.f)); // 5
    PathTraceBRDFShader *shdBRDF3 = new PathTraceBRDFShader(this, "materials/brdfs/grease-covered-steel.binary", Vec3f(1.f)); // 15
    PathTraceBRDFShader *shdBRDF4 = new PathTraceBRDFShader(this, "materials/brdfs/pvc.binary", Vec3f(1.f)); // 20
    PathTraceBRDFShader *shdBRDF5 = new PathTraceBRDFShader(this, "materials/brdfs/color-changing-paint3.binary", Vec3f(1.f)); // 5000
    PathTraceBRDFShader *shdBRDF6 = new PathTraceBRDFShader(this, "materials/brdfs/orange-paint.binary", Vec3f(1.f));
    PathTraceBRDFShader *shdBRDF7 = new PathTraceBRDFShader(this, "materials/brdfs/white-paint.binary", Vec3f(1.f));

    // Set number of bounces (maximum recursive depth).
    shdBRDF1->num_bounces = num_bounces;
    shdBRDF2->num_bounces = num_bounces;
    shdBRDF3->num_bounces = num_bounces;
    shdBRDF4->num_bounces = num_bounces;
    shdBRDF5->num_bounces = num_bounces;
    shdBRDF6->num_bounces = num_bounces;
    shdBRDF7->num_bounces = num_bounces;

    // Load spheres.
    if (task >= 7.1 && task < 7.2)
    {
      // Determine number of lights.
      int num_lights = 3;

      // Add the room (a box). x=-14..-1, y=0..30, z=-6..+6
      this->parseOBJ("materials/models/cube.obj", shdBRDF2, NONORMALS, STANDARD, Vec3f(1), Vec3f(0));

      // Add spheres inside box.
      Sphere *s1 = new Sphere(Vec3f(-9.f, 5.f, 0.f), 2.0, shdBRDF7);
      Sphere *s2 = new Sphere(Vec3f(-7.f, 10.f, 0.f), 1.0, shdBRDF1);
      this->add(s1);
      this->add(s2);

      // Add point light.
      if (1 == num_lights)
      {
        PointLight *pl = new PointLight(this, Vec3f(-5, 25, 0), Vec3f(1000));
        this->add(pl);
      }

      // Add spot light 1 (and remove point light).
      if (num_lights >= 2)
      {
        Vec3f lightPos1(-10, 29, 4);
        Vec3f lightDir1(0, -1, -0.2);
        normalize(lightDir1);
        SpotLight *spotLight1 = new SpotLight(this, lightPos1, lightDir1, 3000, 15.0f, 50.0f); // ... intensity, alpha min/max
        this->add(spotLight1);
      }

      // Add spot light 2 (and still no point light anymore).
      if (num_lights >= 3)
      {
        Vec3f lightPos2(-3.f, s2->center.y, s2->center.z);
        Vec3f lightDir2 = s2->center - lightPos2;
        normalize(lightDir2);
        SpotLight *spotLight2 = new SpotLight(this, lightPos2, lightDir2, 1500, 5.0f, 10.0f); // ... intensity, alpha min/max
        this->add(spotLight2);
      }
    }

    // Undefined task.
    else
    {
      std::cerr << "Undefined task: " << task << std::endl;
      return;
    }
  }

};

#endif
