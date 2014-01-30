#ifndef __RENDERER_H__
#define __RENDERER_H__

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

#include "Common.h"
#include "Object.h"
#include "Scene.h"
#include "Image.h"
#include "Tracer.h"
#include <glm/glm.hpp>

class Renderer {
protected:
	std::shared_ptr<Scene> mScene;
	int mWidth, mHeight, mSamplesPerPixel;
	Image mImage;
public:
	Renderer(int w, int h, int spp, std::shared_ptr<Scene> s) : mWidth(w), mHeight(h), mSamplesPerPixel(spp), mScene(s), mImage(w,h) {}
	virtual void render(const std::string &filename) = 0;
};

class MySpectralRenderer : public Renderer {
protected:
	std::shared_ptr<MySpectralTracer> mTracer;
public:
	MySpectralRenderer(int w, int h, int spp, std::shared_ptr<Scene> s) : Renderer(w,h,spp,s), mTracer(new MySpectralTracer(w,h,s,spp)) {
		mTracer->setBackground(glm::vec3(0.1f,0.3f,0.5f));
	}
	void render(const std::string &filename) {
		for (int y = 0; y < mHeight; y++) {
			for (int x = 0; x < mWidth; x++) {
				mImage[y][x] = mTracer->calculatePixel(x,y); 
			}
			printf("Progress: %6.2f %% rendered... \r", y*mWidth*100.f / (mWidth*mHeight));
		}
		mImage.writePPM(filename.c_str()); 
	}
};

#endif 