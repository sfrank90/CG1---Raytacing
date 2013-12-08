#pragma once
#ifndef SCENE03_H
#define SCENE03_H
#define USE_EXAMPLE_OBJ_MODEL 0

#include "../Scene.h"
#include "../Shader.h"
#include "../01/PerspectiveCamera.h"
#include "../02/PointLight.h"
#include "Sphere.h"
#include "InfinitePlane.h"
#include "Triangle.h"
#include "BRDFShader.h"
#include "PhongShader.h"
#include "EyeLightShader.h"
#include "MirrorShader.h"

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

class Scene03: public Scene
{
	
public:
	bool animation;
  Scene03(int resx, int resy)
      : Scene(resx, resy)
  {
  }

  virtual ~Scene03()
  {
  }

  // --------------------------------------------------------------------------

  /**
   * Builds the scene.
   */
  void build03(const double task)
  {
	animation = false;
    this->camera = new PerspectiveCamera(Vec3f(0, 0, 8), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 60, RESX, RESY);

    // Default: EyeLight surface shaders.
    EyeLightShader *eshd1 = new EyeLightShader(this, Vec3f(1, 0, 0)); // red surface
    EyeLightShader *eshd2 = new EyeLightShader(this, Vec3f(0, 1, 0)); // green surface
    EyeLightShader *eshd3 = new EyeLightShader(this, Vec3f(0, 0, 1)); // blue surface
    EyeLightShader *eshd4 = new EyeLightShader(this, Vec3f(1, 1, 0)); // yellow surface
    EyeLightShader *eshd5 = new EyeLightShader(this, Vec3f(0, 1, 1)); // cyan surface

    // Scene objects.
	Primitive *s1, *s2, *s3, *p1, *t1;
	s1 = new Sphere(Vec3f(-2, 1.7, 0), 2, eshd1);
	s2 = new Sphere(Vec3f(1, -1, 1), 2.2, eshd2);
	s3 = new Sphere(Vec3f(3, 0.8, -2), 2, eshd3);
	p1 = new InfinitePlane(Vec3f(0, -1, 0), Vec3f(0, 1, 0), eshd4);
	t1 = new Triangle(Vec3f(-2, 3, 1), Vec3f(1, 2, 1), Vec3f(3, 2.8, 3), eshd5);

    // Mirror Shader 1.
    if (task == 3.11)
    {
      cout << "Task: " << "Mirror 1 " << task << "\n";
      MirrorShader *mshd1 = new MirrorShader(this, Vec3f(1, 1, 1));
      p1->shader = mshd1;
    }

    // Mirror Shader 2.
    else if (task == 3.12)
    {
      cout << "Task: " << "Mirror 2 " << task << "\n";
      MirrorShader *mshd2 = new MirrorShader(this, Vec3f(1, 1, 1));
      s2->shader = mshd2;
    }

    // Phong.
    else if (task >= 3.2 && task < 3.3)
    {
      // Phong surface shaders.
      cout << "Task: " << "Phong " << task << "\n";
      PhongShader *pshd1 = new PhongShader(this, Vec3f(1, 0, 0), 0.1, 0.5, 0.5, 40); // red surface
      PhongShader *pshd2 = new PhongShader(this, Vec3f(0, 1, 0), 0.1, 0.5, 0.5, 40); // green surface
      PhongShader *pshd3 = new PhongShader(this, Vec3f(0, 0, 1), 0.1, 0.5, 0.5, 40); // blue surface
      PhongShader *pshd4 = new PhongShader(this, Vec3f(1, 1, 0), 0.1, 0.5, 0.5, 40); // yellow surface
      PhongShader *pshd5 = new PhongShader(this, Vec3f(0, 1, 1), 0.1, 0.5, 0.5, 40); // cyan surface
      s1->shader = pshd1;
      s2->shader = pshd2;
      s3->shader = pshd3;
      p1->shader = pshd4;
      t1->shader = pshd5;
    }

    // BRDF Shaders.
    else if (task == 3.3)
    {
      cout << "Task: " << "BRDF " << task << "\n";
      BRDFShader *shdBRDF1 = new BRDFShader(this, "materials/brdfs/green-acrylic.binary", Vec3f(10, 10, 10));
      BRDFShader *shdBRDF2 = new BRDFShader(this, "materials/brdfs/gold-paint.binary", Vec3f(5, 5, 5));
      BRDFShader *shdBRDF3 = new BRDFShader(this, "materials/brdfs/grease-covered-steel.binary", Vec3f(15, 15, 15));
      BRDFShader *shdBRDF4 = new BRDFShader(this, "materials/brdfs/pvc.binary", Vec3f(20, 20, 20));
      BRDFShader *shdBRDF5 = new BRDFShader(this, "materials/brdfs/color-changing-paint3.binary", Vec3f(5000, 5000, 5000));
      s1->shader = shdBRDF1;
      s2->shader = shdBRDF2;
      s3->shader = shdBRDF3;
      p1->shader = shdBRDF4;
      t1->shader = shdBRDF5;
    }
	
	else if(task == 3.15)
	{
		cout << "Task: " << "Model Mirror " << task << "\n";
	}
	else if(task == 3.25)
	{
		cout << "Task: " << "Phong Mirror " << task << "\n";
	}
	else if(task == 3.35)
	{
		cout << "Task: " << " " << task << "\n";
	}

    // Undefined case.
    else
    {
      cout << "Task unknown: " << task << "\n";
      return;
    }

	// Light sources (2 point lights).
	Vec3f pointLightIntensity(50, 50, 50);
	Vec3f lightPosition1(+4, +5, +6);
	Vec3f lightPosition2(-3, +5, 4);
	PointLight *pointLight1 = new PointLight(this, lightPosition1, pointLightIntensity);
	PointLight *pointLight2 = new PointLight(this, lightPosition2, pointLightIntensity);
	this->add(pointLight1);
	this->add(pointLight2);

    // Add primitives to scene.
	if(task == 3.15)
	{	
		animation = true;
		BRDFShader *shdBRDF2 = new BRDFShader(this, "materials/brdfs/gold-paint.binary", Vec3f(5, 5, 5));

		parseOBJ("materials/models/mot1.obj", shdBRDF2, NONORMALS, STANDARD, Vec3f(0.05f,0.05f,0.05f), Vec3f(0.0f,-2.0f,0.0f));
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
		//shaders.erase (shaders.begin()+numTris,shaders.end());

		PhongShader *pshd4 = new PhongShader(this, Vec3f(1, 1, 0), 0.1, 0.5, 0.5, 40);
		p1->shader = pshd4;
		
		MirrorShader *mshd2 = new MirrorShader(this, Vec3f(1, 1, 1));
        s2->shader = mshd2;

		this->add(p1);
		this->add(s2);
				
	} 
	
	else
	{
		this->add(s1);
		this->add(s2);
		this->add(s3);
		this->add(p1);
		this->add(t1);
	}

    

    // Add OBJ model.
#if USE_EXAMPLE_OBJ_MODEL > 0
    parseOBJ("materials/models/cube.obj", eshd5, NONORMALS, STANDARD, Vec3f(0.1f, 0.05f, 0.15f), Vec3f(-1, -.5, 4));
#endif
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
		static int k = 0;
		if(t <= 20)
			k++;
		else 
			k--;
		for(int i = 0; i < primitive.size()-2; i++) {
			((Triangle*)primitive[i])->a = splines[i*3+0].getInterpolatedSplinePoint((k-1)/20.0f);
			((Triangle*)primitive[i])->b = splines[i*3+1].getInterpolatedSplinePoint((k-1)/20.0f);
			((Triangle*)primitive[i])->c = splines[i*3+2].getInterpolatedSplinePoint((k-1)/20.0f);
		}
		std::cout << "k = " << k << std::endl;
	}
	//Primitive before last is a Plane
	InfinitePlane* p = (InfinitePlane*)(primitive[primitive.size()-2]);
	//Last primitve is a Sphere
	Sphere* s = (Sphere*)(primitive[primitive.size()-1]);

	// end part for animation
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
