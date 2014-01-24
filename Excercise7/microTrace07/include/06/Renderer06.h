#pragma once
#ifndef RENDERER06_H
#define RENDERER06_H

#include "../Renderer.h"
#include "Scene06.h"
#include "RegularSampleGenerator.h"
#include "RandomSampleGenerator.h"
#include "StratifiedSampleGenerator.h"
#include "PerspectiveFocalCamera.h"

class Renderer06: public Renderer
{

public:

  SampleGenerator *sampleGenerator;
  int num_samples;

  Renderer06()
      : Renderer(6), sampleGenerator(NULL), num_samples(16)
  {
  }

  ~Renderer06()
  {
    delete sampleGenerator;
  }

  // --------------------------------------------------------------------------

  /**
   * Creates a new sample generator.
   */
  virtual void createSampleGenerator(const double task) {
    // Remove old generator.
    if (NULL != sampleGenerator) {
      delete sampleGenerator;
    }

    // Create regular, random or stratified sampler.
    if (task == 6.31 || task == 6.41) {
      sampleGenerator = new RegularSampleGenerator();
    }
    else if (task == 6.32 || task == 6.42) {
      sampleGenerator = new RandomSampleGenerator();
    }
    else if (task == 6.33 || task == 6.43) {
      sampleGenerator = new StratifiedSampleGenerator();
    }
    else {
      printf("Warning: No sample generator for task %.2f\n", task);
    }
  }

  // --------------------------------------------------------------------------

  /**
   * Renders one frame per task.
   */
  virtual void render(const char *fileName, const double task)
  {
    Scene06 scene = Scene06(RESX, RESY);
    scene.build06(task);

    // Ray Tracing.
    Image img(scene.camera->resX, scene.camera->resY);  // image array
    Ray ray;                                            // primary ray
    Vec3f pixel;                                        // pixel to be accumulated

    // Ray samples.
    float u[MAX_RAY_SAMPLES], v[MAX_RAY_SAMPLES], weight[MAX_RAY_SAMPLES];

    // Progress bar.
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
          pixel.clear();

          // Task 6.3: Oversampling, but no DoF (depth-of-field).
          if (task >= 6.3 && task < 6.4)
          {
            sampleGenerator->getSamples(num_samples, u, v, weight);       // Get sample positions.
            for (int i = 0; i < num_samples; i++)                         // Shoot one primary ray per sample and accumulate.
            {
              scene.camera->initRay(x + 0.0 + u[i], y + 0.0 + v[i], ray); // init ray, no +0.5 this time, because u, v are [0..1]
              pixel = pixel + weight[i] * scene.rayTrace(ray);            // add weighted color
            }
          }

          // Task 6.4: DoF (depth-of-field).
          else if (task >= 6.4 && task < 6.5)
          {
            sampleGenerator->getSamples(num_samples, u, v, weight);       // Get sample positions.
            for (int i = 0; i < num_samples; i++)                         // Shoot one primary ray per sample and accumulate.
            {
              scene.camera->initRay(x + .5, y + .5, u[i] - .5, v[i] - .5, ray); // init ray
              pixel = pixel + weight[i] * scene.rayTrace(ray);                  // add weighted color
            }
          }

          // No oversampling for earlier tasks.
          else
          {
            scene.camera->initRay(x + 0.5, y + 0.5, ray);
            pixel =scene.rayTrace(ray);
          }

          img[y][x] = clampComponent(pixel, 0, 1);  // Color values must be clamped between 0 and 1.
          pixel_done++;                             // Add 1 to number of pixels done for progess calculation.
        }
        printf("(MicroTrace): %6.2f %% rendered... at t=%d\r", pixel_done * percent_per_pixel, t);
      }
      printf("(MicroTrace): Rendering task %.2f done (t=%4d).\n", task, t);
      img.writePPM(fileNameN.c_str()); // write image for time t.
    }
  }

};

#endif
