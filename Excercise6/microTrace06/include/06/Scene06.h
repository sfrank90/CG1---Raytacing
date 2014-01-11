#pragma once
#ifndef SCENE06_H
#define SCENE06_H

#include "../01/PerspectiveCamera.h"
#include "../03/EyeLightShader.h"
#include "../05/Scene05.h"
#if ENABLE_TEXTURES
#include "../08/Texture.h"
#include "../08/TexturedEyeLightShader.h"
#endif
#include "PerspectiveFocalCamera.h"
#include "Sphere.h"
#include "CheckeredPlane.h"
#include "CheckeredEyeLightShader.h"

class Scene06: public Scene05
{
public:

  Scene06(int resx, int resy)
      : Scene05(resx, resy)
  {
  }

  virtual ~Scene06()
  {
  }

  // --------------------------------------------------------------------------
  /**
   * Builds the scene.
   */
  void build06(const double task)
  {
    // Task 6.3: Load checkerboard ground.
    if (task >= 6.3 && task < 6.4)
    {
#if ENABLE_TEXTURES
      // Note: before 2013, the texture exercise was before this one.
      Texture* texture = new Texture("materials/textures/cb.ppm", nearest);
      this->add(texture);
      TexturedEyeLightShader* sh = new TexturedEyeLightShader(this, texture, STANDARDUV, Vec3f(5, 5, 5));
      this->parseOBJ("materials/models/ground.obj", sh, TEXTURENORMALS, SMOOTH, Vec3f(1), Vec3f(0));
#else
      CheckeredEyeLightShader *eshdc = new CheckeredEyeLightShader(this, .5);
      CheckeredPlane *plane = new CheckeredPlane(Vec3f(0), Vec3f(0, 1, -.1), Vec3f(0, .1, 1), eshdc);
      this->add(plane);
#endif
      this->camera = new PerspectiveCamera(Vec3f(0, 0, -3), Vec3f(0, 0, 1), Vec3f(0, 1, 0), 60, resx, resy);
    }

    // Task 6.4: Load 3 balls.
    else if (task >= 6.4 && task < 6.5)
    {
      EyeLightShader *eshd1 = new EyeLightShader(this, Vec3f(1, 0, 0)); // red surface
      EyeLightShader *eshd2 = new EyeLightShader(this, Vec3f(0, 1, 0)); // green surface
      EyeLightShader *eshd3 = new EyeLightShader(this, Vec3f(0, 0, 1)); // blue surface
      Sphere *s1 = new Sphere(Vec3f(+2.0, +0.0, +1.0), 1, eshd1);
      Sphere *s2 = new Sphere(Vec3f(-0.0, +0.0, +5.0), 1, eshd2);
      Sphere *s3 = new Sphere(Vec3f(-5.0, +0.0, +9.0), 1, eshd3);
      this->add(s1);
      this->add(s2);
      this->add(s3);
      this->camera = new PerspectiveFocalCamera(Vec3f(0, 0, -3), Vec3f(0, 0, 1), Vec3f(0, 1, 0), 60, 0.4, /*(+3 + 5.0) / 2.*/ 8.f, RESX, RESY);
    }

    // Task 5.1: kd-Tree.
    if (task >= 5.0)
    {
      this->buildAccelerationStructure();
    }
  }

};

#endif
