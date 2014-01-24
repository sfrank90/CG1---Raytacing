#pragma once
#ifndef RENDERER07_H
#define RENDERER07_H

#include "../Renderer.h"
#include "../06/StratifiedSampleGenerator.h"
#include "Scene07.h"

class Renderer07: public Renderer
{

protected:

  SampleGenerator *sampleGenerator;

public:

  Renderer07()
      : Renderer(7), sampleGenerator(new StratifiedSampleGenerator())
  {
  }

  ~Renderer07()
  {
    delete sampleGenerator;
  }

  /**
   * Renders one frame per task.
   */
  virtual void render(const char *fileName, const double task)
  {
    if (num_samples > MAX_RAY_SAMPLES)
    {
      printf("Too many ray samples (%d, max: %d).\n", num_samples, MAX_RAY_SAMPLES);
      return;
    }

    num_bounces = 1 + (((int) (task * 1000.)) % 100) / 10; // stupid double precision, would just use %10 otherwise
    std::cout << "(Main) Using " << num_bounces << " bounces, with " << num_samples << " samples." << std::endl;

    // Build scene.
    Scene07 scene = Scene07(RESX, RESY);
    scene.build07(task, num_bounces);

    // Ray Tracing.
    Image img(scene.camera->resX, scene.camera->resY); // image array
    Ray ray; // primary ray
    Vec3f pixel = Vec3f(0); // pixel to be accumulated
    float percent_per_pixel = 100.f / (scene.camera->resX * scene.camera->resY); // percentage for process
    int pixel_done = 0;
    float u[MAX_RAY_SAMPLES], v[MAX_RAY_SAMPLES], weight[MAX_RAY_SAMPLES];

    // Iterate over all frames, allpixels.
    for (int t = 1; t <= NUM_FRAMES; t++)
    {
      scene.tick(t); // update scene
      std::string fileNameN = createBounceImageFileName(fileName, num_bounces, num_samples, t); // get filename for frame t
      pixel_done = 0;
      for (int y = 0; y < scene.camera->resY; y++)
      {
        for (int x = 0; x < scene.camera->resX; x++)
        {
          pixel.clear();
          sampleGenerator->getSamples(num_samples, u, v, weight);
          for (int i = 0; i < num_samples; i++) // Shoot one primary ray per sample and accumulate.
          {
            scene.camera->initRay(x + 0.0 + u[i], y + 0.0 + v[i], ray); // init ray, no +0.5 this time, because u, v are [0..1]
            pixel += weight[i] * scene.rayTrace(ray); // add weighted color
          }
          img[y][x] = clampComponent(pixel, 0, 1); // clamp to [0,1]
          pixel_done++; // add 1 to number of pixels done for progress calculation
        }
        printf("(MicroTrace): %6.2f %% rendered... at t=%d\r", pixel_done * percent_per_pixel, t);
      }
      printf("(MicroTrace): Rendering task %.2f done (t=%4d).\n", task, t);
      img.writePPM(fileNameN.c_str()); // write image for time t.
    }
  }

};

#endif
