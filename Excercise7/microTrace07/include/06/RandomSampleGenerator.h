#pragma once
#ifndef RANDOMSAMPLEGENERATOR_H
#define RANDOMSAMPLEGENERATOR_H

#include "SampleGenerator.h"

class RandomSampleGenerator: public SampleGenerator
{
public:

  virtual void getSamples(int n, float *u, float *v, float *weight)
  {
    for (int i = 0; i < n; i++)
    {
      u[i] = rand() / (float) RAND_MAX;
      v[i] = rand() / (float) RAND_MAX;
      weight[i] = 1.0 / (float) n;
    }
  }

};

#endif
