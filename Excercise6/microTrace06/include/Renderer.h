#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "Vec3f.h"
#include "Image.h"

class Renderer
{

public:

  int task;
  int num_samples;
  int num_bounces;

  Renderer(int task)
      : task(task), num_samples(NUM_SAMPLES), num_bounces(0)
  {
    srand(1); // it's perfectly random, i've thrown a dice...
  }

  virtual ~Renderer()
  {
  }

  /**
   * Renders one frame per task.
   */
  virtual void render(const char *fileName, const double task) = 0;

  // --------------------------------------------------------------------------

  /**
   * Returns a good filename for a time step.
   */
  const std::string createImageFileName(const char* fileName, int step)
  {
    std::stringstream ss;
    ss << strndup(fileName, strlen(fileName) - 4);
    ss << "_";
    ss << std::setfill('0');
    ss << std::setw(4);
    ss << step;
    ss << ".ppm";
    return ss.str();
  }

  // --------------------------------------------------------------------------

  /**
   * Returns a good filename for a number of bounces and samples.
   */
  const std::string createBounceImageFileName(const char* fileName, int bounces, int samples, int step)
  {
    std::stringstream ss;
    ss << strndup(fileName, strlen(fileName) - 4);
    ss << "_s";
    ss << std::setfill('0');
    ss << std::setw(4);
    ss << samples;
    ss << "_b";
    ss << std::setfill('0');
    ss << std::setw(4);
    ss << bounces;
    ss << "_t";
    ss << std::setfill('0');
    ss << std::setw(4);
    ss << step;
    ss << ".ppm";
    return ss.str();
  }

};

#endif
