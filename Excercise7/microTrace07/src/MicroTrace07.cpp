#define SHEET 7
#define RESX 640                  // image x-resolution
#define RESY 480                  // image y-resolution
#define NUM_FRAMES 1              // number of frames to be rendered.
#define ENABLE_TEXTURES false     // whether textures are supported.
#define ENABLE_KDTREE true        // whether kd-tree acceleration is supported.
#define NUM_SAMPLES 64            // number of samples for lights
#define MAX_RECURSION_LEVEL 30    // recursion level for reflections and refractions
#define MAX_RAY_SAMPLES 1024      // maximum number of samples per ray
// ----------------------------------------------------------------------------
#include "07/Renderer07.h"

/**
 * Main entry point. Use with "-samples <num>" parameter to determine the
 * number of ray samples per pixel.
 */
int main(int argc, char* argv[])
{
  Renderer07 re;
  if (argc >= 3 && !strcmp(argv[1], "-samples"))
  {
    re.num_samples = atoi(argv[2]);
    re.render("output/box_and_spheres.ppm", 7.10);
    re.render("output/box_and_spheres.ppm", 7.11);
    re.render("output/box_and_spheres.ppm", 7.12);
  }
  else
  {
    for (int i = 1; i <= 8; i++)
    {
      re.num_samples = pow(pow(2, i), 2);
      re.render("output/box_and_spheres.ppm", 7.10);
      re.render("output/box_and_spheres.ppm", 7.11);
      re.render("output/box_and_spheres.ppm", 7.12);
    }
  }
}
