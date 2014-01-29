#pragma once
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <glm/glm.hpp>

class Image
{
protected:

  int resX, resY;
  glm::vec3 *pixel;

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
    pixel = new glm::vec3[resX * resY];
  }

  ~Image()
  {
    delete[] pixel;
  }

  glm::vec3 *operator[](int y)
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
