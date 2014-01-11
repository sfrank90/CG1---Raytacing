#pragma once
#ifndef PRIMITIVE01_H
#define PRIMITIVE01_H

#include "../Primitive.h"

class Primitive01 : public Primitive
{
public:

  Primitive01()
  {
  }

  virtual ~Primitive01()
  {
  }

  virtual bool findIntersection(Ray &ray) = 0;

};

#endif
