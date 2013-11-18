#pragma once
#ifndef IMAGE_H
#define IMAGE_H

#include "Vec3f.h"

class Image
{
protected:

  int resX, resY;
  Vec3f *pixel;

public:

  Image(const char* filename)
  {
    pixel = NULL;
    readPPM(filename);
  }

  Image(int resX, int resY)
      : resX(resX), resY(resY)
  {
    assert(resX > 0 && resY > 0);
    pixel = new Vec3f[resX * resY];
  }

  ~Image()
  {
    delete[] pixel;
  }

  Vec3f *operator[](int y)
  {
    return pixel + y * resX;
  }

  /**
   * Writes this image into PPM file.
   */
  void writePPM(const char *fileName);

  /**
   * Reads a PPM file into this image. The image must be 3-channel.
   */
  void readPPM(const char* filename);

};

#endif
