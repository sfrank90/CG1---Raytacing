#pragma once
#ifndef RENDERER04_H
#define RENDERER04_H

#include "../Renderer.h"
#include "../Image.h"
#include "Scene04.h"

class Renderer04: public Renderer
{

public:

  Renderer04()
      : Renderer(4)
  {
  }

  ~Renderer04()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Renders one frame per task.
   */
  virtual void render(const char *fileName, const double task)
  {
    // Define a scene.
    Scene04 scene = Scene04(RESX, RESY);
    scene.build04(task);

    // Ray Tracing.
    Image img(scene.camera->resX, scene.camera->resY); // image array
    Ray ray;                                           // primary ray

    float percent_per_pixel = 100.f / (scene.camera->resX * scene.camera->resY); // percentage for process
    int pixel_done = 0;

    for (int t = 1; t <= NUM_FRAMES; t++)
    {
      scene.tick(t); // update scene
      std::string fileNameN = createImageFileName(fileName, t); // get filename for frame t
      pixel_done = 0;
      for (int y = 0; y < scene.camera->resY; y++)
      {
        for (int x = 0; x < scene.camera->resX; x++)
        {
          scene.camera->initRay(x + 0.5, y + 0.5, ray); // initialize ray

          // Color values must be clamped between 0 and 1.
          img[y][x] = clampComponent(scene.rayTrace(ray), 0, 1);

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
