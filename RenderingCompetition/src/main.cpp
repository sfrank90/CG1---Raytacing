/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/

#include "../include/Scene.h"
#include "../include/Renderer.h"
#include "../include/Camera.h"
#include "../include/Objects/Sphere.h"
#include "../include/Objects/Plane.h"
#include "../include/Objects/TriangleMesh.h"
#include "../include/Materials/Diffuse.h"
#include "../include/Light.h"
#include "../include/model_obj.h"
#include <iostream>
#include <sstream>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

const int iWidth = 640;
const int iHeight = 480;
const int iSamples = 6;

void createCornelBox(Scene *scene) {
	Material* green = new Material;
	Material* red = new Material;
	Material* white = new Material;

	green->mDiffuse = glm::vec3(0.0f, 0.9f, 0.0f);
	green->mSpecular = glm::vec3(0.f);
	green->shininess = 0.f;
	red->mDiffuse = glm::vec3(0.9f, 0.0f, 0.0f);
	red->mSpecular = glm::vec3(0.f);
	red->shininess =0.f;
	white->mDiffuse = glm::vec3(0.9f);
	white->mSpecular = glm::vec3(0.f);
	white->shininess = 0.f;
	white->mTransmission = glm::vec3(0.0f);
	white->shininess = 60.f;

	float highExtend = 7.5;
	std::vector<Object*> planes, refines;
	Object* left = new Plane(glm::vec3(1.f,0.f,0.f), 
		                       glm::vec3(-7.5f,-7.0f,-7.5f), 
							   glm::vec3(-7.5f,7.0f,-7.5f),
							   glm::vec3(-7.5f,7.0f,7.5f),
							   glm::vec3(-7.5f,-7.0f,7.5f), red);
	Object* right = new Plane(glm::vec3(-1.f,0.f,0.f), 
		                       glm::vec3(7.5f,-7.0f,-7.5f), 
							   glm::vec3(7.5f,7.0f,-7.5f),
							   glm::vec3(7.5f,7.0f,7.5f),
							   glm::vec3(7.5f,-7.0f,7.5f), green);
	Object* top = new Plane(glm::vec3(0.f,-1.f,0.f), 
		                       glm::vec3(-7.5f,7.0f,-7.5f), 
							   glm::vec3(7.5f,7.0f,-7.5f),
							   glm::vec3(7.5f,7.0f,7.5f),
							   glm::vec3(-7.5f,7.0f,7.5f), white);
	Object* bottom = new Plane(glm::vec3(0.f,1.f,0.f), 
		                       glm::vec3(-7.5f,-7.0f,-7.5f), 
							   glm::vec3(7.5f,-7.0f,-7.5f),
							   glm::vec3(7.5f,-7.0f,7.5f),
							   glm::vec3(-7.5f,-7.0f,7.5f), white);
	Object* back = new Plane(glm::vec3(0.f,0.f,1.f), 
		                       glm::vec3(-7.5f,-7.0f,-7.5f), 
							   glm::vec3(7.5f,-7.0f,-7.5f),
							   glm::vec3(7.5f,7.0f,-7.5f),
							   glm::vec3(-7.5f,7.0f,-7.5f), white);
	planes.push_back(left);
	planes.push_back(right);
	planes.push_back(top);
	planes.push_back(bottom);
	planes.push_back(back);
	
	for(int i=0; i <planes.size(); ++i) {
		planes[i]->refine(refines); //create triangles out of planes
	}
	for(int i = 0; i < refines.size(); ++i) {
		scene->addObject(refines[i]);
	}

	//// create quadlight (need some visual expression)
	//float extend = 2.f;
	//glm::vec3 normal = glm::vec3(0.f, -1.f, 0.f);
	//Light* light = new QuadAreaLight(scene, 
	//	                             7.f*glm::vec3(1.f, 1.f, 1.f),  //power
	//								 glm::vec3(-extend, 7.0-2*constants::epsilon, -extend),
	//								 glm::vec3(extend,  7.0-2*constants::epsilon, -extend),
	//								 glm::vec3(extend,  7.0-2*constants::epsilon, extend),
	//								 glm::vec3(-extend, 7.0-2*constants::epsilon, extend));
	//
	////add blackbody distribution here,...maybe...
	//scene->addLight(light);
	//
	//Object* t1   = new Triangle(glm::vec3(-extend, 7.0-constants::epsilon, -extend),
	//							glm::vec3(extend,  7.0-constants::epsilon, -extend),
	//							glm::vec3(extend,  7.0-constants::epsilon, extend),
	//							normal,normal,normal,white,true);
	//Object* t2   = new Triangle(glm::vec3(extend,  7.0-constants::epsilon, -extend),
	//							glm::vec3(extend,  7.0-constants::epsilon, extend),
	//							glm::vec3(-extend, 7.0-constants::epsilon, extend),
	//							normal,normal,normal,white,true);

	////visual expression, here it comes
	//scene->addObject(t1);
	//scene->addObject(t2);
	Object* lightsphere(new Sphere(glm::vec3(0.0f, 5.6f, 0.f), 1.f-constants::epsilon, white, true));
	Light* lightbulb = new PointLight(scene, glm::vec3(0.0f, 5.6f, 0.f),5.f*glm::vec3(1.f, 1.f, 1.f), 1.f);
	scene->addObject(lightsphere);
	scene->addLight(lightbulb);

}

int main(int argc, char* argv[]) {
	Camera *camera = new PerspectiveCamera(glm::vec3(0.f,0.f,20.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f,1.f,0.f), 45.f, iWidth, iHeight);
	Scene* scene = new Scene();
	scene->setCamera(camera);
	scene->enableShadow(true);
	//Create distributions, materials, sphere and light

	//Distribution* constantDistr(new ConstantDistribution(600.f)); //600nm between orange and red...
	//Distribution* blackbodyDistr(new BlackBodyDistribution(5200.f)); //5200 kelvin, pretty bright

	//diffuse sphere
	Material* diffuse = new Material;//(new Diffuse(constantDistr)); 
	diffuse->mDiffuse = glm::vec3(0.8f, 0.1f, 0.1f);
	diffuse->mSpecular = glm::vec3(0.1f);
	diffuse->mTransmission = glm::vec3(0.f);
	diffuse->shininess = 30.f;
	//Material* diffuse2 = new Material;//(new Diffuse(constantDistr)); 
	//diffuse2->mAbsorptionCoeff = 0.2;
	//diffuse2->mScatterCoeff = 0.5;
	//diffuse2->mDiffuse = glm::vec3(0.2f, 0.7f, 0.2f);
	//diffuse2->mSpecular = glm::vec3(0.f);//glm::vec3(0.8f, 0.1f, 0.2f);
	//diffuse2->mTransmission = glm::vec3(0.0f);
	//diffuse2->shininess = 0.f;
	//diffuse2->mParticitatingMedium = false;

	Material* diffuse3 = new Material;//(new Diffuse(constantDistr)); 
	diffuse3->mDiffuse = glm::vec3(0.01f);
	diffuse3->mSpecular = glm::vec3(0.0f);
	diffuse3->mTransmission = glm::vec3(1.0f);
	diffuse3->shininess = 60.f;
	diffuse3->ior = 1.5003f;

	//need albedo for correct colorness... mDiffuse is used as albedo but should rather be a diffusion probability
	Material* buddhaMaterial = new Material;//(new Diffuse(constantDistr)); 
	buddhaMaterial->mDiffuse = glm::vec3(0.1f, 0.1f, 0.7f);
	buddhaMaterial->mSpecular = glm::vec3(0.3f);
	buddhaMaterial->mTransmission = glm::vec3(1.0f);
	buddhaMaterial->shininess = 96.f;
	buddhaMaterial->ior = 1.3303f;

	std::shared_ptr<ModelOBJ> mObj(new ModelOBJ());
	if(!mObj->import("buddha.obj"))  {
		std::cerr << "Could not load" << std::endl;
		return 0;
	}
	//mObj->normalize(1.8);

	TriangleMesh* buddha = new TriangleMesh(buddhaMaterial);
	buddha->setData(mObj);

	std::vector<Object*> refines;
	buddha->refine(refines, glm::translate(glm::vec3(0.f,-7.f,1.f)));
	for(int i = 0; i < refines.size(); ++i) {
		scene->addObject(refines[i]);
	}

	Object* sphere(new Sphere(glm::vec3(3.6f, -4.8f, -5.f), 2.f, diffuse));
	Object* sphere2(new Sphere(glm::vec3(-3.6f, -4.3f, -5.f), 2.3f, diffuse3));

	scene->addObject(sphere);
	scene->addObject(sphere2);

	//create cornell box
	createCornelBox(scene);

	//only create acc. structure if there are enough primitves
	std::cout <<scene->getObjects().size() << " intersectable primitves!" << std::endl;
	if(scene->getObjects().size() > 4)
		scene->buildAccelerationStructure();

	MySimpleRenderer renderer(iWidth, iHeight, iSamples, scene, 
						      8,     //Max no. of bounces
							  true,  //global illumination with photon mapping?
							  150000, //max no. of global photons
							  150000, //max no. of caustic photons
							  10000, //max no. of volume photons for participating media (not yet implemented..., so won't have any effect)
							  300000);//max photon shots per light
	//get timestamp for unique filename
    unsigned long long tp= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	std::stringstream str;
	str << "results/result" << tp << ".ppm";
	//let's go!
	renderer.render(str.str());
	
	return 0;
}