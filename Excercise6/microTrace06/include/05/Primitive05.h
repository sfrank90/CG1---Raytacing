#pragma once
#ifndef PRIMITIVE05_H
#define PRIMITIVE05_H

#include "../03/Primitive03.h"
#include "Box.h"

class Primitive05: public Primitive03
{
public:

  Primitive05(Shader *shader = NULL)
      : Primitive03(shader)
  {
  }

  virtual ~Primitive05()
  {
  }

  /**
   * Calculates the bounding box of the primitive.
   */
  virtual Box calcBounds() = 0;

  /**
   * Determines whether the primitive is within the given box.
   */
  virtual bool isInVoxel(Box &b)
  {
    return calcBounds().overlapsBox(b);
  }

};

#endif
