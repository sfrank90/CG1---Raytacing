#pragma once
#ifndef SAMPLEGENERATOR_H
#define SAMPLEGENERATOR_H

class SampleGenerator
{
public:

  SampleGenerator()
  {
  }

  virtual ~SampleGenerator()
  {
  }

  /**
   * Generates n samples in the range 0..1 that whose x/y coordinates
   * and relative weights are saved in u, v, and weights.
   */
  virtual void getSamples(int n, float *u, float *v, float *weight) = 0;

};

#endif
