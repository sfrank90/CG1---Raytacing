#pragma once
#ifndef RENDERER01_H
#define RENDERER01_H

#include "../Renderer.h"
#include "../Image.h"
#include "PerspectiveCamera.h"
#include "Triangle.h"
#include "Sphere.h"
#include "InfinitePlane.h"

class Renderer01: public Renderer
{

protected:

public:

  Renderer01()
      : Renderer(1)
  {
  }

  ~Renderer01()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Renders one frame per task.
   */
  virtual void render(const char *fileName, const double task)
  {
    // Load scene description.
    Sphere s1(Vec3f(-2, 1.7, 0), 2);
    Sphere s2(Vec3f(1, -1, 1), 2.2);
    Sphere s3(Vec3f(3, 0.8, -2), 2);
    InfinitePlane p1(Vec3f(0, -1, 0), Vec3f(0, 1, 0));
    Triangle t1(Vec3f(-2, 3.7, 0), Vec3f(1, 2, 1), Vec3f(3, 2.8, -2));
    Triangle t2(Vec3f(3, 2, 3), Vec3f(3, 2, -3), Vec3f(-3, 2, -3));

    // Create camera.
    PerspectiveCamera *camera = NULL;
    if (task == 1.31)
    {
      camera = new PerspectiveCamera(Vec3f(0, 0, 10), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 60, RESX, RESY);
    }
    else if (task == 1.32)
    {
      camera = new PerspectiveCamera(Vec3f(-8, 3, 8), Vec3f(1, -.1, -1), Vec3f(0, 1, 0), 45, RESX, RESY);
    }
    else if (task == 1.33)
    {
      camera = new PerspectiveCamera(Vec3f(-8, 3, 8), Vec3f(1, -.1, -1), Vec3f(1, 1, 0), 45, RESX, RESY);
    }
    else
    {
      cout << "Task not recognized: " << task << endl;
      return;
    }

    // Ray Tracing.
    Image img(camera->resX, camera->resY); // image array
    Ray ray; // primary ray

    float percent_per_pixel = 100.f / (camera->resX * camera->resY); // percentage for process
    int pixel_done = 0;

    for (int y = 0; y < camera->resY; y++)
    {
      for (int x = 0; x < camera->resX; x++)
      {
        Vec3f col = Vec3f(0, 0, 0); // background color

        // This is the heart of your ray tracer. The two "for"-loops iterate over
        // each pixel in the final image and assign it a colour according to the
        // object that is closest to the camera.
        // Closest means: The object that has the shortest distance on a ray shot
        // through that pixel. If no object is intersected, then the ray is
        // simply assigned the background color (which is black in our case).
        //
        // (1) Initialize your ray here by calling Camera::initRay with your
        //     current pixel (x,y).
        // (2) Initialize a color for the current pixel with background color.
        // (3) Now "iterate" over your scene objects and intersect them with the
        //     ray. The ::findIntersection(Ray &ray) method returns true, if the object
        //     is currently closest the camera. In this case you have to change
        //     the pixel color according to the object color
        //     (with Primitive::getColor())

        camera->initRay(x + 0.5, y + 0.5, ray); // initialize ray

        if (s1.findIntersection(ray)) col = Vec3f(1, 0, 0);
        if (s2.findIntersection(ray)) col = Vec3f(0, 1, 0);
        if (s3.findIntersection(ray)) col = Vec3f(0, 0, 1);
        if (p1.findIntersection(ray)) col = Vec3f(1, 1, 0);
        if (t1.findIntersection(ray)) col = Vec3f(0, 1, 1);
        if (t2.findIntersection(ray)) col = Vec3f(1, 1, 1);

        img[y][x] = col; // store pixel color
        pixel_done++; // add 1 to number of pixels done for progess calculation
      }
      printf("(MicroTrace): %6.2f %% rendered...\r", pixel_done * percent_per_pixel);
    }
    printf("\n(MicroTrace): Rendering done!\n");

    img.writePPM(fileName); // write final image
  }

};

#endif
