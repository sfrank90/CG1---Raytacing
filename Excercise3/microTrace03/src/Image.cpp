#include "Image.h"

/**
 * Writes this image into PPM file.
 */
void Image::writePPM(const char *fileName)
{
  std::ofstream file(fileName);
  file << "P3" << std::endl;
  file << resX << " " << resY << " " << 255 << std::endl;
  for (int y = resY - 1; y >= 0; y--)
  {
    for (int x = 0; x < resX; x++)
    {
      file << (int) (255.99999999 * (*this)[y][x].x) << " " << (int) (255.99999999 * (*this)[y][x].y) << " " << (int) (255.99999999 * (*this)[y][x].z) << " " << "\t";
    }
    file << std::endl;
    file << std::flush;
  }
  printf("(Image): written file: %s\n", fileName);
}

/**
 * Removes white space.
 */
void eatWhiteSpaces(std::ifstream& file)
{
  char c;
  c = file.peek();
  while (c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\r')
  {
    file.get(c);
    c = file.peek();
  }
}

/**
 * Removes comments.
 */
void eatComments(std::istream& file)
{
  // Peek for comments.
  char c;
  std::string comment;
  c = file.peek();
  while (c == '#')
  {
    std::getline(file, comment);
    c = file.peek();
  }
}

/**
 * Reads a PPM file into this image. The image must be 3-channel.
 */
void Image::readPPM(const char* filename)
{
  // Implement this to read a texture from disk.
  std::cerr << "(Image): Reading file " << filename << std::endl;
  std::ifstream file(filename);

  if (!file.is_open())
  {
    std::cerr << "(Image): Could not open file " << filename << std::endl;
    return;
  }

  eatWhiteSpaces(file);
  eatComments(file);
  eatWhiteSpaces(file);

  std::string magic;
  file >> magic;

  eatWhiteSpaces(file);
  eatComments(file);
  eatWhiteSpaces(file);

  int depth;
  file >> resX >> resY >> depth;

  // Allocate RAM for image.
  if (pixel != NULL)
  {
    delete[] pixel;
    pixel = NULL;
  }
  pixel = new Vec3f[resX * resY];

  int r, g, b;

  for (int y = resY - 1; y >= 0; --y)
  {
    for (int x = 0; x < resX; ++x)
    {
      file >> r >> g >> b;
      Vec3f color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f);
      (*this)[y][x] = color;
    }
  }

  file.close();
}

