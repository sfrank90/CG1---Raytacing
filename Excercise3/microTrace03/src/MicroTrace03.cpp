#define SHEET 3
#define RESX 640                  // image x-resolution
#define RESY 480                  // image y-resolution
#define NUM_FRAMES 40             // number of frames to be rendered.
#define ENABLE_TEXTURES false     // whether textures are supported.
#define NUM_SAMPLES 1000          // number of samples for lights
#define MAX_RECURSION_LEVEL 30    // recursion level for reflections and refractions
// ----------------------------------------------------------------------------
#include "03/Renderer03.h"

/**
 * Main method. Use with "-phong", "-mirror" or "-brdf".
 */
int main(int argc, char* argv[])
{
  Renderer03 re;

  // Task 3.1.
  if (argc >= 2 && !strcmp(argv[1], "-mirror"))
  {
    re.render("output/mirror1.ppm", 3.11);
    re.render("output/mirror2.ppm", 3.12);
  }

  // Task 3.2
  else if (argc >= 2 && !strcmp(argv[1], "-phong"))
  {
    re.render("output/phong.ppm", 3.2);
  }

  // Task 3.3.
  else if (argc >= 2 && !strcmp(argv[1], "-brdf"))
  {
    re.render("output/brdf.ppm", 3.3);
  }

  // Task "Use Model"
  else if (argc >= 2 && !strcmp(argv[1], "-models"))
  {
    re.render("output/mirror-model.ppm", 3.15);
	re.render("output/phong-model.ppm", 3.25);
	re.render("output/brdf-model.ppm", 3.35);
  }

  // All.
  else
  {
    re.render("output/mirror1.ppm", 3.11);
    re.render("output/mirror2.ppm", 3.12);
    re.render("output/phong.ppm", 3.2);
    re.render("output/brdf.ppm", 3.3);
  }
}
