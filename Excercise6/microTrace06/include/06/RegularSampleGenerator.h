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
	
	float step = 1.f/m;
	float center = 0.5f*step;
    // IMPLEMENT ME 6.1.1
    // IMPLEMENT ME END.
	for(unsigned int i = 0; i < m; ++i) {
		for(unsigned int j = 0; j < m; ++j) {
			v[i*m + j] = center + i*step; //shoot through center of each sample point
			u[i*m + j] = center + j*step;
			weight[i*m + j] = 1.f/(n); //equally distributed
		}
	}
  }

};

#endif
