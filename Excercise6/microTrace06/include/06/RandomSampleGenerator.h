#pragma once
#ifndef RANDOMSAMPLEGENERATOR_H
#define RANDOMSAMPLEGENERATOR_H

#include "SampleGenerator.h"

class RandomSampleGenerator: public SampleGenerator
{
public:

  virtual void getSamples(int n, float *u, float *v, float *weight)
  {
    // IMPLEMENT ME 6.1.2
    // IMPLEMENT ME END.

	  srand (static_cast <unsigned> (time(0)));
	  for(int i = 0; i < n; i++) {
		float x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - EPSILON; //first sample point [0,1[
		float y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - EPSILON; // second sample point [0,1[
		u[i] = x;
		v[i] = y;
		weight[i] = 1.f/n; //? always use average?
	  }
  }

};

#endif
