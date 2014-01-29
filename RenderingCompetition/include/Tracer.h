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
	~Tracer() {}
	
	virtual glm::vec3 calculatePixel(int x, int y) const = 0;

protected:
	std::uint32_t mWidth;
	std::uint32_t mHeight;

	std::shared_ptr<Scene> mScene;
};

class MyTracer : public Tracer {
public:
	MyTracer(int w, int h, std::shared_ptr<Scene> s, int pixel_subdivs = 1)
	: Tracer(w, h, s), subdivs(pixel_subdivs), mBackground(1.0) { 
		computeJitters();
	}

	void setBackground(const glm::vec3& color) { mBackground = color; }

	void incrementPixelSubdivs();
	void decrementPixelSubdivs();

	virtual glm::vec3 calculatePixel(int x, int y) const;

	//const Shader *getShader(const geometry::Ray &ray) const;
protected:
	void computeJitters();

	int subdivs;
	std::vector<glm::vec2> jitter;
	glm::vec2 win_to_vp;
	glm::vec2 lower_left;
	glm::vec2 step;
	glm::vec3 mBackground;
};

#endif
