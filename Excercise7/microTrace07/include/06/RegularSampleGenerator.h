#pragma once
#ifndef REGULARSAMPLEGENERATOR_H
#define REGULARSAMPLEGENERATOR_H

#include "SampleGenerator.h"

class RegularSampleGenerator: public SampleGenerator
{
public:

  virtual void getSamples(int n, float *u, float *v, float *weight)
  {
    int m = (int) sqrt((float) n);
    if (m * m != n)
    {
      std::cerr << "regular sampling only possible with a square number of samples" << std::endl;
      exit(1);
    }

    for (int i = 0; i < m; i++)
    {
      for (int j = 0; j < m; j++)
      {
        u[i * m + j] = (i + 0.5) / (float) m;
        v[i * m + j] = (j + 0.5) / (float) m;
        weight[i * m + j] = 1.0 / (float) n;
      }
    }
  }

};

#endif
