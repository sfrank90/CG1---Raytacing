#pragma once
#ifndef RENDERER02_H
#define RENDERER02_H

#include "../Renderer.h"
#include "../Image.h"
#include "Scene02.h"

class Renderer02: public Renderer
{

protected:

public:

  Renderer02()
      : Renderer(2)
  {
  }

  ~Renderer02()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Returns a good filename for a time step.
   */
  const string createImageFileName(const char* fileName, int step)
  {
    stringstream ss;
    ss << strndup(fileName, strlen(fileName) - 4);
    ss << "_";
    ss << setfill('0');
    ss << setw(4);
    ss << step;
    ss << ".ppm";
    return ss.str();
  }

  // --------------------------------------------------------------------------

  /**
   * Renders one frame per task.
   */
  virtual void render(const char *fileName, const double task)
  {
    // Define a scene with camera.
    Scene02 scene = Scene02(RESX, RESY);
    scene.build02(task);

    // Ray Tracing.
    Image img(scene.camera->resX, scene.camera->resY); // image array
    Ray ray;                                           // primary ray

    float percent_per_pixel = 100.f / (scene.camera->resX * scene.camera->resY); // percentage for process
    int pixel_done = 0;

    for (int t = 1; t <= NUM_FRAMES; t++)
    {
      scene.tick(t); // update scene
      string fileNameN = createImageFileName(fileName, t); // get filename for frame t
      pixel_done = 0;
      for (int y = 0; y < scene.camera->resY; y++)
      {
        for (int x = 0; x < scene.camera->resX; x++)
        {
          scene.camera->initRay(x + 0.5, y + 0.5, ray); // initialize ray

          // Color values must be clamped between 0 and 1.
          img[y][x] = clampComponent(scene.rayTrace2(ray), 0, 1);

          // Add 1 to number of pixels done for progess calculation.
          pixel_done++;
        }
        printf("(MicroTrace): %6.2f %% rendered... at t=%d\r", pixel_done * percent_per_pixel, t);
      }
      printf("(MicroTrace): Rendering task %.2f done (t=%4d).\n", task, t);
      img.writePPM(fileNameN.c_str()); // write image for time t.
    }
  }

};

#endif
