#define SHEET 2
#define RESX 640                  // image x-resolution
#define RESY 480                  // image y-resolution
#define ENABLE_TEXTURES 0     // whether textures are supported.
#define NUM_SAMPLES 1000          // number of samples for lights
#define MAX_RECURSION_LEVEL 30    // recursion level for reflections and refractions
// ----------------------------------------------------------------------------
#include "02/Renderer02.h"

/**
 * Main method. Takes no parameters.
 */
int main(int argc, char* argv[])
{
  Renderer02 re;
  re.render("output/pointlight.ppm", 2.3);
}
