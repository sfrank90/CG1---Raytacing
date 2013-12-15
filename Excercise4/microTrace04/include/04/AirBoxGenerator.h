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

    // IMPLEMENT ME 4.3
    // (1) Assign Khular's parameters.
    // (2) Return a refractive index for a given height according to Khular's
    //     formula.
    // IMPLEMENT ME END

    return retVal;
  }

public:

  AirBoxGenerator(double spacing, Scene* scene, double ground, double atmosphereEnd)
      : scene(scene), spacing(spacing), ground(ground), atmosphereEnd(atmosphereEnd)
  {
  }

  void generatePlanes()
  {
    // IMPLEMENT ME 4.3
    // (1) Generate a discrete set of equally spaced infinite planes.
    // (2) Loop between ground level and the upper end.
    // (3) Iterate until height is higher than atmosphere end.
    // IMPLEMENT ME END
  }

};

#endif
