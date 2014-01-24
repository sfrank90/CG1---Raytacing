#ifndef AIRBOXGENERATOR_H
#define AIRBOXGENERATOR_H

#include "../Scene.h"
#include "../03/InfinitePlane.h"
#include "RefractiveShader.h"

#define RAND()  0

class AirBoxGenerator
{

private:

  Scene *scene;
  double spacing;
  double ground;
  double atmosphereEnd;

  double getIndexforHeight(double height)
  {
    // We need to divide here, because our scene is too small.
    // This is a workaround for making a nice picture,
    // it does not belong to the formula of Khular et al.
    height = height / 1000.f;
    double retVal;

    // (1) Assign Khular's parameters.
    // (2) Return a refractive index for a given height according to Khular's
    //     formula.
    double mu0 = 1.00233; // Ground height
    double mu1 = 0.4584;  // Constants as in Khular et al. 1977
    double beta = 2.303;  // Constants as in Khular et al. 1977
    retVal = sqrt((mu0 * mu0) + (mu1 * mu1) * (1 - exp(-beta * height)));

    return retVal;
  }

public:

  AirBoxGenerator(double spacing, Scene* scene, double ground, double atmosphereEnd)
      : scene(scene), spacing(spacing), ground(ground), atmosphereEnd(atmosphereEnd)
  {
  }

  void generatePlanes()
  {
    // (1) Generate a discrete set of equally spaced infinite planes.
    // (2) Loop between ground level and the upper end.
    // (3) Iterate until height is higher than atmosphere end.

    // Generate a discrete set of equally spaced infinite planes.
    double height = ground + spacing;
    double oldheight = ground;

    // Loop between ground level and the upper end.
    do
    {
      RefractiveShader* shdRef = new RefractiveShader(scene, Vec3f(1.0, 1.0, 1.0), 1.0, getIndexforHeight(oldheight - ground), getIndexforHeight(height - ground));

      // Create an infinite plane for the current height with the shader,
      // add it to scene.
      InfinitePlane* p1 = new InfinitePlane(Vec3f(0, height, 0), Vec3f(0, 1, 0), shdRef);
      scene->add(p1);

      // Update the heights.
      oldheight = height;
      height = height + spacing;

      // Iterate until height is higher than atmosphere end.
    } while (height < atmosphereEnd);
  }

};

#endif
