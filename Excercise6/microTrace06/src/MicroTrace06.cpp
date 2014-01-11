#define SHEET 6
#define RESX 640                  // image x-resolution
#define RESY 480                  // image y-resolution
#define NUM_FRAMES 1              // number of frames to be rendered.
#define ENABLE_TEXTURES false     // whether textures are supported.
#define ENABLE_KDTREE true        // whether kd-tree acceleration is supported.
#define NUM_SAMPLES 1             // number of samples for lights
#define MAX_RECURSION_LEVEL 30    // recursion level for reflections and refractions
#define MAX_RAY_SAMPLES 1024      // maximum number of samples per ray
// ----------------------------------------------------------------------------
#include "06/Renderer06.h"

/**
 * Main entry point. Use with "-regular", "-random" or "-stratified"
 * parameter to determine type of oversampling.
 */
int main(int argc, char* argv[])
{
  Renderer06 re;
  if (argc >= 3 && !strcmp(argv[1], "-regular"))
  {
    re.num_samples = atoi(argv[2]);
    re.createSampleGenerator(6.31);
    if (argc >= 4 && !strcmp(argv[3], "-dof"))
    {
      re.render("output/sampling_04_dof_regular.ppm", 6.41);
    }
    else
    {
      re.render("output/sampling_01_regular.ppm", 6.31);
    }
  }
  else if (argc >= 3 && !strcmp(argv[1], "-random"))
  {
    re.num_samples = atoi(argv[2]);
    re.createSampleGenerator(6.32);
    if (argc >= 4 && !strcmp(argv[3], "-dof"))
    {
      re.render("output/sampling_05_dof_random.ppm", 6.42);
    }
    else
    {
      re.render("output/sampling_02_random.ppm", 6.32);
    }
  }
  else if (argc >= 3 && !strcmp(argv[1], "-stratified"))
  {
    re.num_samples = atoi(argv[2]);
    re.createSampleGenerator(6.33);
    if (argc >= 4 && !strcmp(argv[3], "-dof"))
    {
      re.render("output/sampling_06_dof_stratified.ppm", 6.43);
    }
    else
    {
      re.render("output/sampling_03_stratified.ppm", 6.33);
    }
  }
  else if (argc == 1)
  {
    printf("(MicroTrace): No arguments specified, rendering the images from the assignment.\n");
    re.createSampleGenerator(6.31);
    re.num_samples = 1;
    re.render("output/sampling_00_none.ppm", 6.31);
    re.num_samples = 16;
    re.render("output/sampling_01_regular.ppm", 6.31);
    re.render("output/sampling_04_dof_regular.ppm", 6.41);
    re.createSampleGenerator(6.32);
    re.render("output/sampling_02_random.ppm", 6.32);
    re.render("output/sampling_05_dof_random.ppm", 6.43);
    re.createSampleGenerator(6.33);
    re.render("output/sampling_03_stratified.ppm", 6.33);
    re.render("output/sampling_06_dof_stratified.ppm", 6.43);
  }
  else
  {
    std::cerr << "Usage: " << argv[0] << " [option(s)]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Options are:" << std::endl;
    std::cerr << "  -regular <samples>    : Regular sampling." << std::endl;
    std::cerr << "  -random <samples>     : Random sampling." << std::endl;
    std::cerr << "  -stratified <samples> : Stratified sampling." << std::endl;
    exit(1);
  }

}
