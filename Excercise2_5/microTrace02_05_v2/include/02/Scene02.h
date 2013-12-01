#pragma once
#ifndef SCENE02_H
#define SCENE02_H
#define USE_EXAMPLE_OBJ_MODEL 0

#include "../Scene.h"
#include "../01/PerspectiveCamera.h"
#include "Sphere.h"
#include "Triangle.h"
#include "InfinitePlane.h"
#include "PointLight.h"

//Some fun with splines (CatMulRom)
class Spline {
public:

    // Constructors and destructor
    Spline() : vp(), delta_t(0) {}
    Spline(const Spline& s){
		for (int i = 0; i < (int)s.vp.size(); i++)
			vp.push_back(s.vp[i]);
		delta_t = s.delta_t;
	}

    // Operations
    void addSplinePoint(const Vec3f& v) {
		vp.push_back(v);
		delta_t = (float)1 / (float)vp.size();
	}
    Vec3f getInterpolatedSplinePoint(float t)	{ // t = 0...1; 0=vp[0] ... 1=vp[max]
		// Find out in which interval we are on the spline
		int p = (int)(t / delta_t);
		// Compute local control point indices

		int p0 = p - 1;     bounds(p0);
		int p1 = p;         bounds(p1);
		int p2 = p + 1;     bounds(p2);
		int p3 = p + 2;     bounds(p3);

		// Relative (local) time
		float lt = (t - delta_t*(float)p) / delta_t;
		// Interpolate
		return Spline::eq(lt, vp[p0], vp[p1], vp[p2], vp[p3]);
	}
    int getNumPoints() {
		return vp.size();
	}
    Vec3f& getNthPoint(int n) {
		return vp[n];
	}

    // Static method for computing the Catmull-Rom parametric equation
    // given a time (t) and a vector quadruple (p1,p2,p3,p4).
    static Vec3f eq(float t, const Vec3f& p1, const Vec3f& p2,  const Vec3f& p3, const Vec3f& p4) {
		float t2 = t * t;
		float t3 = t2 * t;

		float b1 = .5 * (  -t3 + 2*t2 - t);
		float b2 = .5 * ( 3*t3 - 5*t2 + 2);
		float b3 = .5 * (-3*t3 + 4*t2 + t);
		float b4 = .5 * (   t3 -   t2    );

		return (p1*b1 + p2*b2 + p3*b3 + p4*b4); 
	}

private:
	//helper
	void bounds(int &pp) {
		if (pp < 0) 
			pp = 0;  
		else if (pp >= (int)vp.size()-1) 
			pp = vp.size() - 1;
	}
    std::vector<Vec3f> vp;
    float delta_t;
};

class Scene02: public Scene
{

public:

  Scene02(int resx, int resy)
      : Scene(resx, resy)
  {
  }

  virtual ~Scene02()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Builds the scene.
   */
  void build02(const double task)
  {
    this->camera = new PerspectiveCamera(Vec3f(0, 0, 8), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 60, RESX, RESY);

    // Light sources.
    Vec3f pointLightIntensity(30, 30, 30);
    Vec3f lightPosition1(4, 3, 6);
    PointLight* pointLight1 = new PointLight(this, lightPosition1, pointLightIntensity);

    // Load scene description.
    Sphere* s1 = new Sphere(Vec3f(-2, 1.7, 0), 2, pointLight1, Vec3f(1, 0, 0));
    Sphere* s2 = new Sphere(Vec3f(1, -1, 1), 2.2, pointLight1, Vec3f(0, 1, 0));
    Sphere* s3 = new Sphere(Vec3f(3, 0.8, -2), 2, pointLight1, Vec3f(0, 0, 1));
    InfinitePlane* p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), pointLight1, Vec3f(1, 1, 0));
    Triangle* t1 = new Triangle(Vec3f(-2, 3, 1), Vec3f(1, 2, 1), Vec3f(3, 2.8, 3), pointLight1, Vec3f(0, 1, 1));

    // Add primitives to scene.
    /*add(s1);
    add(s2);
    add(s3);
    add(p1);
    add(t1);*/
    add(pointLight1);
	//test
	parseOBJ("materials/models/mot1.obj", NULL, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-2.0f,0.0f));
	int numTris = (int)primitive.size();
	int numSamplePoints = 5;
	parseOBJ("materials/models/mot2.obj", NULL, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-2.0f,0.0f));
	parseOBJ("materials/models/mot3.obj", NULL, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-2.0f,0.0f));
	parseOBJ("materials/models/mot4.obj", NULL, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-2.0f,0.0f));
	parseOBJ("materials/models/mot5.obj", NULL, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-2.0f,0.0f));

	splines.resize(numTris*3);

	for(int i = 0; i < numTris; i++) {
		for(int j = 0; j < numSamplePoints; j++) {
			splines[i*3+0].addSplinePoint(((Triangle*)primitive[numTris*j + i])->a);
			splines[i*3+1].addSplinePoint(((Triangle*)primitive[numTris*j + i])->b);
			splines[i*3+2].addSplinePoint(((Triangle*)primitive[numTris*j + i])->c);
		}
	}

	primitive.erase (primitive.begin()+numTris,primitive.end());
    // IMPLEMENT ME 2.4
    // Make an OBJ model with Blender and add it using the parseOBJ method.
    //
    // Use NULL as shader, NONORMALS as object style, STANDARD
    // as triangle style, and scale and translation parameters as fit.
    // If you need rotation, go to Scene::parseOBJ() and extend it.
    // IMPLEMENT ME END
  }

  // --------------------------------------------------------------------------

  /**
   * Trace the given ray and shade it; return the color of the shaded ray.
   */
  Vec3f rayTrace2(Ray &ray)
  {
    // Check if the ray intersects an object in the scene.
    return findIntersection(ray) ? ((Primitive02*) ray.hit)->TraceLightRay(ray) : bgColor;
  }

  // --------------------------------------------------------------------------

  /**
   * Advances the camera and object state by one.
   */
  void tick(int t)
  {
    PerspectiveCamera *cam = (PerspectiveCamera*) this->camera;
    /*Sphere *s1 = (Sphere*) this->primitive[0];
    Sphere *s2 = (Sphere*) this->primitive[1];*/

    // IMPLEMENT ME 2.3
    // Make both camera position/direction, and some objects move.
    // IMPLEMENT ME END

	//Animation

	static int k = 0;

	if(t <= 20)
		k++;
	else 
		k--;

	for(int i = 0; i < primitive.size(); i++) {
		((Triangle*)primitive[i])->a = splines[i*3+0].getInterpolatedSplinePoint((k-1)/20.0f);
		((Triangle*)primitive[i])->b = splines[i*3+1].getInterpolatedSplinePoint((k-1)/20.0f);
		((Triangle*)primitive[i])->c = splines[i*3+2].getInterpolatedSplinePoint((k-1)/20.0f);
	}

	std::cout << "k = " << k << std::endl;

    cam->initAxes();
  }

  // --------------------------------------------------------------------------

  /**
   * Creates a basic (unshaded, untextured, non-smooth) triangle.
   */
  virtual Primitive* createTrianglePrimitive(Shader* shd, TriangleStyle tstyle, OBJStyle objstyle, Vec3f* tri, Vec3f* norm, Vec3f* tex)
  {
    return new Triangle(tri[0], tri[1], tri[2], lights[0], Vec3f(0, 1, 1)); // cyan
  }

};

#endif
