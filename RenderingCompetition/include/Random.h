#pragma once
#ifndef __RANDOM_H__
#define __RANDOM_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/

#include <random>

template <typename T, int F1, int F2>
class RandomNumber
{
private:
	std::mt19937 mTwister;
	std::uniform_real_distribution<> mDistribution;
public:
	RandomNumber() {
		std::random_device rd;
		mTwister = std::mt19937(rd());
	    mDistribution = std::uniform_real_distribution<>((double)F1, (double)F2);
	}

	T operator()() {
		return (T)mDistribution(mTwister);
	}

};

typedef RandomNumber<float, 0, 1> RandomFloat;

#endif
