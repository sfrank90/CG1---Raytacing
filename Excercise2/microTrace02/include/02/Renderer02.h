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
      : Renderer(1)
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
    // ss << strndup(fileName, strlen(fileName) - 4); //bad code
	std::string strFile(fileName);
	strFile.erase(strFile.end()-4, strFile.end());
	ss << strFile;
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

    for (int t = 1; t <= 40; t++)
    {
      scene.tick(t); // update scene.
      string fileNameN = createImageFileName(fileName, t);
      for (int y = 0; y < scene.camera->resY; y++)
      {
        for (int x = 0; x < scene.camera->resX; x++)
        {
          scene.camera->initRay(x + 0.5, y + 0.5, ray); // initialize ray

          // Color values must be clamped between 0 and 1.
          img[y][x] = minComponent(maxComponent(scene.rayTrace2(ray), Vec3f(0, 0, 0)), Vec3f(1, 1, 1));

          // Add 1 to number of pixels done for progess calculation.
          pixel_done++;
        }
        printf("(MicroTrace): %6.2f %% rendered... at t=%d\r", pixel_done * percent_per_pixel, t);
      }
      printf("\n(MicroTrace): Rendering done (t=%4d)!\n", t);
      img.writePPM(fileNameN.c_str()); // write image for time t.
    }
  }

};

#endif
