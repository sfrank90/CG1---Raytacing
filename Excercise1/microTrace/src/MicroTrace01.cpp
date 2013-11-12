#define SHEET 1
#define RESX 640 // image x-resolution
#define RESY 480 // image y-resolution
#define NUM_SAMPLES 0
// ----------------------------------------------------------------------------
#include "01/Renderer01.h"

/**
 * Render three images with different cameras and settings.
 */
int main(int argc, char* argv[])
{
  Renderer01 re;
  re.render("output/perspective1.ppm", 1.31);
  re.render("output/perspective2.ppm", 1.32);
  re.render("output/perspective3.ppm", 1.33);
}
