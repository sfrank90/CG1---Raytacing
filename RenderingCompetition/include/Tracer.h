#ifndef __TRACER_H__
#define __TRACER_H__

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*
*/

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Scene.h"
#include "Ray.h"

class  Tracer {
public:
	Tracer(int w, int h, std::shared_ptr<Scene> s) : mWidth(w), mHeight(h), mScene(s) {}
	virtual ~Tracer() {}
	
	virtual glm::vec3 calculatePixel(int x, int y) const = 0;

protected:
	std::uint32_t mWidth;
	std::uint32_t mHeight;

	std::shared_ptr<Scene> mScene;
};

class MySpectralTracer : public Tracer {
public:
	MySpectralTracer(int w, int h, std::shared_ptr<Scene> s, int pixel_subdivs = 1)
	: Tracer(w, h, s), subdivs(pixel_subdivs), mBackground(0.0) { 
		computeJitters();
	}
	virtual ~MySpectralTracer() {}

	void setBackground(const glm::vec3& color) { mBackground = color; }

	void incrementPixelSubdivs();
	void decrementPixelSubdivs();

	virtual glm::vec3 calculatePixel(int x, int y) const;
	
	//const Shader *getShader(const geometry::Ray &ray) const;
protected:
	void computeJitters();
	float estimateRadiance(Ray &ray, float wavelength) const;

	int subdivs;
	std::vector<glm::vec2> jitter;
	glm::vec2 win_to_vp;
	glm::vec2 lower_left;
	glm::vec2 step;
	glm::vec3 mBackground;
};

class MyPhotonTracer : public MySpectralTracer {
public:
	MyPhotonTracer(int w, int h, std::shared_ptr<Scene> s, int pixel_subdivs = 1)
	: MySpectralTracer(w, h, s, pixel_subdivs) { }
	virtual ~MyPhotonTracer() {}

	//virtual glm::vec3 calculatePixel(int x, int y) const;
	//void buildMaps();
protected:
};

#endif
