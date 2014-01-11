#pragma once
#ifndef STRATIFIEDSAMPLEGENERATOR_H
#define STRATIFIEDSAMPLEGENERATOR_H

#include "SampleGenerator.h"

class StratifiedSampleGenerator: public SampleGenerator
{
public:

  /**
   * Generates n samples in the range 0..1 that whose x/y coordinates
   * and relative weights are saved in u, v, and weights.
   */
  virtual void getSamples(int n, float *u, float *v, float *weight)
  {
    int m = (int) sqrt((float) n);
    if (m * m != n)
    {
      std::cerr << "stratified sampling only possible with a square number of samples" << std::endl;
      exit(1);
    }

    // IMPLEMENT ME 6.1.3
    // IMPLEMENT ME END.
  }

};

#endif
