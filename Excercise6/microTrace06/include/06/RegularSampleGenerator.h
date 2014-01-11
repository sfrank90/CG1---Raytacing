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

    // IMPLEMENT ME 6.1.1
    // IMPLEMENT ME END.
  }

};

#endif
