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

    float step = 1.f/m;
	//float center = 0.5f*step;
    // IMPLEMENT ME 6.1.3
    // IMPLEMENT ME END.
	for(unsigned int i = 0; i < m; ++i) {
		for(unsigned int j = 0; j < m; ++j) {
		    float x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 1e-4; //first sample point in subpixel [0,1[
		    float y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 1e-4; // second sample point in subpixel [0,1[
			v[i*m + j] = (i+y)*step; //shoot through random point of this subpixel
			u[i*m + j] = (j+x)*step;
			weight[i*m + j] = 1.f/(n); //equally distributed
		}
	}
  }

};

#endif
