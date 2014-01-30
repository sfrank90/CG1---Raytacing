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
#include <iostream>

const int iWidth = 640;
const int iHeight = 480;
const int iSamples = 1;

int main(int argc, char* argv[]) {
	std::shared_ptr<Camera> camera(new PerspectiveCamera(glm::vec3(0.f,0.f,1.f), glm::vec3(0.f), glm::vec3(0.f,1.f,0.f), 45.f, iWidth, iHeight));
	std::shared_ptr<Scene> scene(new Scene());
	scene->setCamera(camera);
	//scene->buildAccelerationStructure();

	MySpectralRenderer renderer(iWidth, iHeight, iSamples, scene);

	renderer.render("results/firsttest.ppm");
	
	return 0;
}