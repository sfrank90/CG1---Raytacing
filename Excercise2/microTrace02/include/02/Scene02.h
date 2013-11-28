#pragma once
#ifndef SCENE02_H
#define SCENE02_H

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
private:
	Spline mCamPosition;
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
    add(s1);
    add(s2);
    add(s3);
    add(p1);
    add(t1);
    add(pointLight1);

	//Add spline points
	mCamPosition.addSplinePoint(Vec3f(0, 0, 8));
	mCamPosition.addSplinePoint(Vec3f(1, 0, 7));
	mCamPosition.addSplinePoint(Vec3f(2, 1, 6));
	mCamPosition.addSplinePoint(Vec3f(3, 2, 5));
	mCamPosition.addSplinePoint(Vec3f(4, 3, 4));
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
    Sphere *s1 = (Sphere*) this->primitive[0];
    Sphere *s2 = (Sphere*) this->primitive[1];

    // IMPLEMENT ME 2.3
    // Make both camera position/direction, and some objects move.
    // IMPLEMENT ME END

	static const float pi = 3.14159265f;

	//rotate spheres around (0,0,0)
	float oldXS1 = s1->center.x; float oldZS1 = s1->center.z;
	float oldXS2 = s2->center.x; float oldZS2 = s2->center.z;
	float da = 1.0f/20.0f;

	if(t > 20) {
		s1->center.x =  oldXS1 * cos(pi*da) - oldZS1 * sin(pi*da);
		s1->center.z =  oldXS1 * sin(pi*da) + oldZS1 * cos(pi*da);
		s2->center.x =  oldXS2 * cos(pi*da) - oldZS2 * sin(pi*da);
		s2->center.z =  oldXS2 * sin(pi*da) + oldZS2 * cos(pi*da);
	}

	//use splines to calculate new camera position
	Vec3f newCamPos = mCamPosition.getInterpolatedSplinePoint(t/20.0f);
	cam->pos = newCamPos;
	cam->dir = Vec3f(0,0,0) - newCamPos;
	normalize(cam->dir);
    cam->initAxes();
  }

  // --------------------------------------------------------------------------

  /**
   * Ignore for now. We do not have shaders yet.
   */
  virtual Primitive* createTrianglePrimitive(Shader* shd, TriangleStyle tstyle, OBJStyle objstyle, Vec3f* tri, Vec3f* norm, Vec3f* tex)
  {
    return NULL;
  }

};

#endif
