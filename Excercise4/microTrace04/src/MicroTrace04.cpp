#define SHEET 4
#define RESX 640                  // image x-resolution
#define RESY 480                  // image y-resolution
#define NUM_FRAMES 1             // number of frames to be rendered.
#define ENABLE_TEXTURES false     // whether textures are supported.
#define NUM_SAMPLES 1             // number of samples for lights
#define MAX_RECURSION_LEVEL 30    // recursion level for reflections and refractions
// ----------------------------------------------------------------------------
#include "04/Renderer04.h"

/**
 * Main method. Use with "-refraction" or "-mirages".
 */
int main(int argc, char* argv[])
{
  Renderer04 re;

  // Task 4.1.
  if (argc >= 2 && !strcmp(argv[1], "-refraction"))
  {
    re.render("output/refraction.ppm", 4.1);
  }

  // Task 4.2.
  if (argc >= 2 && !strcmp(argv[1], "-spot"))
  {
    re.render("output/spot_light.ppm", 4.2);
  }

  // Task 4.3.
  else if (argc >= 2 && !strcmp(argv[1], "-mirages"))
  {
    re.render("output/mirages.ppm", 4.3);
  }
  else if (argc >= 2 && !strcmp(argv[1], "-animation"))
  {
    re.render("output/animation.ppm", 4.4);
  }

  // All.
  else
  {
    re.render("output/refraction.ppm", 4.1);
    re.render("output/spot_light.ppm", 4.2);
    // re.render("output/mirages.ppm", 4.3); // deactivated WS13/14.
	//re.render("output/spot_light.ppm", 4.5);
  }
}
