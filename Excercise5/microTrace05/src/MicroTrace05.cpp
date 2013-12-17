#define SHEET 5
#define RESX 640                  // image x-resolution
#define RESY 480                  // image y-resolution
#define NUM_FRAMES 1              // number of frames to be rendered.
#define ENABLE_TEXTURES false     // whether textures are supported.
#define ENABLE_KDTREE true        // whether kd-tree acceleration is supported.
#define NUM_SAMPLES 1             // number of samples for lights
#define MAX_RECURSION_LEVEL 30    // recursion level for reflections and refractions
// ----------------------------------------------------------------------------
#include "05/Renderer05.h"

/**
 * Main method. Takes no parameters.
 */
int main(int argc, char* argv[])
{
  Renderer05 re;
  re.render("output/teapot_in_a_stadium.ppm", 5.1);
}
