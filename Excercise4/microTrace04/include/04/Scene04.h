#pragma once
#ifndef SCENE04_H
#define SCENE04_H
#define USE_EXAMPLE_OBJ_MODEL 0

#include "../Scene.h"
#include "../01/PerspectiveCamera.h"
#include "../02/PointLight.h"
#include "../03/FlatShader.h"
#include "../03/PhongShader.h"
#include "../03/BRDFShader.h"
#include "../03/Triangle.h"
#include "../03/Sphere.h"
#include "../03/InfinitePlane.h"
#include "AirBoxGenerator.h"
#include "RefractiveShader.h"
#include "SpotLight.h"

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

class Scene04: public Scene
{
private:
	std::vector<Spline> splines;
	Spline mCamPosition;
	bool animation;
public:

  Scene04(int resx, int resy)
      : Scene(resx, resy)
  { 
	  animation = false;
  }

  virtual ~Scene04()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Builds the scene.
   */
  void build04(const double task)
  {
    this->camera = new PerspectiveCamera(Vec3f(0, 0.8, 11), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 60, resx, resy);

    // Load scene description.
    if (task >= 4.3 && task < 4.4)
    { // mirages.
      AirBoxGenerator ag(0.05, this, -1.0, 2.0);
      ag.generatePlanes();
      this->camera = new PerspectiveCamera(Vec3f(0, 0.2, 60), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 8, resx, resy);
    }

    // Phong/flat/BRDF shaders.
    PhongShader *shd1 = new PhongShader(this, Vec3f(1, 0, 0), 0.1, 0.5, 0.5, 40); // red surface
    PhongShader *shd2 = new PhongShader(this, Vec3f(0, 1, 0), 0.1, 0.5, 0.5, 40); // green surface
    PhongShader *shd3 = new PhongShader(this, Vec3f(0, 0, 1), 0.1, 0.5, 0.5, 40); // blue surface
    PhongShader *shd4 = new PhongShader(this, Vec3f(1, 1, 0), 0.1, 0.5, 0.5, 40); // yellow surface
    PhongShader *shd5 = new PhongShader(this, Vec3f(0, 1, 1), 0.1, 0.5, 0.5, 40); // cyan surface
    FlatShader *fshd = new FlatShader(this, Vec3f(1, 1, 0));
    BRDFShader *shdBRDF1 = new BRDFShader(this, "materials/brdfs/green-acrylic.binary", Vec3f(10, 10, 10));

    // Scene objects.
    Primitive *s1, *s2, *s3, *p1, *t1;

    // Scene objects (primitives).
    s1 = new Sphere(Vec3f(-2, 1.7, 0), 2, shd1);
    s3 = new Sphere(Vec3f(3, 0.8, -2), 2, shd3);
    if (task >= 4.1 && task < 4.2)
    { // refraction
      RefractiveShader *shdRef1 = new RefractiveShader(this, Vec3f(1.0, 1.0, 1.0), 1.0, 1.0, 1.3);
      p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), shd4);
      s2 = new Sphere(Vec3f(1, -1, 1), 2.2, shdRef1);
    }
    else if (task >= 4.3 && task < 4.4)
    { // mirages
      p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), fshd);
      s2 = new Sphere(Vec3f(1, -1, 1), 2.2, shd2);
    }
    else if (task >= 4.2 && task < 4.3)
    { // spotlight
      p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), shd4);
      s2 = new Sphere(Vec3f(1, -1, 1), 2.2, shdBRDF1);
    }
	else if(task >= 4.4 && task < 4.5) {
		delete s1; delete s3;
		RefractiveShader *shdRef1 = new RefractiveShader(this, Vec3f(1.0, 1.0, 1.0), 1.0, 1.0, 1.3);
		RefractiveShader *shdRef2 = new RefractiveShader(this, Vec3f(0.2, 0.2, 1.0), 1.0, 1.0, 1.55277);
		RefractiveShader *shdRef3 = new RefractiveShader(this, Vec3f(1.0, 0.2, 0.2), 1.0, 1.0, 3.49713);
		s1 = new Sphere(Vec3f(0.0, 0.8, 5), 1.5, shdRef1);
		s2 = new Sphere(Vec3f(0.0, 0.8, -5), 1.5, shdRef2);
		s3 = new Sphere(Vec3f(5, 0.8, 0.0), 1.5, shdRef3);

        p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), shd4);
		animation = true;
	}
    else
    {
      p1 = s2 = NULL;
      cout << "Task unknown: " << task << "\n";
      return;
    }
    t1 = new Triangle(Vec3f(-2, 3, 1), Vec3f(1, 2, 1), Vec3f(3, 2.8, 3), shd5);

    // Add primitives to scene.
	if(task >= 4.4 && task < 4.5) {
		//BRDFShader *shdBRDF2 = new BRDFShader(this, "materials/brdfs/gold-paint.binary", Vec3f(5, 5, 5));

		parseOBJ("materials/models/mot1.obj", shdBRDF1, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-1.0f,0.0f));
		/*int numTris = (int)primitive.size();
		int numSamplePoints = 5;
		parseOBJ("materials/models/mot2.obj", NULL, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-1.0f,0.0f));
		parseOBJ("materials/models/mot3.obj", NULL, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-1.0f,0.0f));
		parseOBJ("materials/models/mot4.obj", NULL, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-1.0f,0.0f));
		parseOBJ("materials/models/mot5.obj", NULL, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-1.0f,0.0f));

		splines.resize(numTris*3);

		for(int i = 0; i < numTris; i++) {
			for(int j = 0; j < numSamplePoints; j++) {
				splines[i*3+0].addSplinePoint(((Triangle*)primitive[numTris*j + i])->a);
				splines[i*3+1].addSplinePoint(((Triangle*)primitive[numTris*j + i])->b);
				splines[i*3+2].addSplinePoint(((Triangle*)primitive[numTris*j + i])->c);
			}
		}
		primitive.erase (primitive.begin()+numTris,primitive.end());*/
		
		this->add(p1);
		this->add(s1);
		this->add(s2);
		this->add(s3);

		mCamPosition.addSplinePoint(Vec3f(0, 0.8, 11));
		//mCamPosition.addSplinePoint(Vec3f(1, 0, 7));
		mCamPosition.addSplinePoint(Vec3f(11, 0.8, 0));
		//mCamPosition.addSplinePoint(Vec3f(3, 2, 5));
		mCamPosition.addSplinePoint(Vec3f(0, 0.8, -11));
	} else {
		this->add(s1);
		this->add(s2);
		this->add(s3);
		this->add(p1);
		this->add(t1);
	}

    // Add lights.
    Vec3f pointLightIntensity(50);
    Vec3f lightPosition1(4, 5, 6);
    Vec3f lightPosition2(-3, 5, +4);
    if (task >= 4.2 && task < 4.3)
    { // spot light
      Vec3f spotLightSourceIntensity1(150, 150, 150);
      Vec3f spotLightSourceIntensity2(50, 50, 50);
      Vec3f lightDir1 = Vec3f(1, -1, 1) - lightPosition1;
      Vec3f lightDir2 = Vec3f(-2, -1, 2) - lightPosition2;
      normalize(lightDir1);
      normalize(lightDir2);
      float alpha_min1 = 15.0f, alpha_max1 = 30.0f;
      float alpha_min2 = 5.0f, alpha_max2 = 10.0f;
      SpotLight *spotLight1 = new SpotLight(this, lightPosition1, lightDir1, spotLightSourceIntensity1, alpha_min1, alpha_max1);
      SpotLight *spotLight2 = new SpotLight(this, lightPosition2, lightDir2, spotLightSourceIntensity2, alpha_min2, alpha_max2);
      this->add(spotLight1);
      this->add(spotLight2);
    }
    else
    {
      PointLight *pointLight1 = new PointLight(this, lightPosition1, pointLightIntensity);
      PointLight *pointLight2 = new PointLight(this, lightPosition2, pointLightIntensity);
      this->add(pointLight1);
      this->add(pointLight2);
    }
  }

  // --------------------------------------------------------------------------

  /**
   * Advances the camera and object state by one.
   */
  void tick(int t)
  {
    PerspectiveCamera *cam = (PerspectiveCamera*) this->camera;

	//ticks for animation
	if(animation)
	{
		/*static int k = 0;
		if(t <= 20)
			k++;
		else 
			k--;
		for(int i = 0; i < primitive.size()-4; i++) {
			((Triangle*)primitive[i])->a = splines[i*3+0].getInterpolatedSplinePoint((k-1)/20.0f);
			((Triangle*)primitive[i])->b = splines[i*3+1].getInterpolatedSplinePoint((k-1)/20.0f);
			((Triangle*)primitive[i])->c = splines[i*3+2].getInterpolatedSplinePoint((k-1)/20.0f);
		}
		std::cout << "k = " << k << std::endl;*/

		/*Sphere *s2 = (Sphere*) this->primitive[primitive.size()-1];
		float pi = 3.14159265359;
		float oldXS2 = s2->center.x; float oldZS2 = s2->center.z;
		float da = 1.0f/20.0f;

		if(t > 1) {
			s2->center.x =  oldXS2 * cos(pi*da) - oldZS2 * sin(pi*da);
			s2->center.z =  oldXS2 * sin(pi*da) + oldZS2 * cos(pi*da);
		}*/

		//Primitives
		InfinitePlane* p1 = (InfinitePlane*)(primitive[primitive.size()-4]);
		Sphere* s1 = (Sphere*)(primitive[primitive.size()-3]);
		Sphere* s2 = (Sphere*)(primitive[primitive.size()-2]);
		Sphere* s3 = (Sphere*)(primitive[primitive.size()-1]);

		Vec3f newCamPos = mCamPosition.getInterpolatedSplinePoint(t/40.0f);
		cam->pos = newCamPos;
		cam->dir = Vec3f(0,0,0) - newCamPos;
		normalize(cam->dir);
    }
	cam->initAxes();
  }

  // --------------------------------------------------------------------------

  /**
   * Creates a basic (untextured, non-smooth) triangle.
   */
  virtual Primitive* createTrianglePrimitive(Shader* shd, TriangleStyle tstyle, OBJStyle objstyle, Vec3f* tri, Vec3f* norm, Vec3f* tex)
  {
    return new Triangle(tri[0], tri[1], tri[2], shd);
  }

};

#endif
