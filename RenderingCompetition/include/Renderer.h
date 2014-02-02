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

#include <omp.h>

class Renderer {
protected:
	Scene* mScene;
	int mWidth, mHeight, mSamplesPerPixel;
	Image mImage;
public:
	Renderer(int w, int h, int spp, Scene* s) : mWidth(w), mHeight(h), mSamplesPerPixel(spp), mScene(s), mImage(w,h) {}
	virtual void render(const std::string &filename) = 0;
};

class MySimpleRenderer : public Renderer {
protected:
	MySimpleTracer *mTracer;
	int mMaxShotsPerLight;
public:
	MySimpleRenderer(int w, int h, int spp, Scene* s, int numBounces=5, bool globalIllumination=false, int globalPhotons=10000, int causticPhotons=10000, int volumePhotons=10000, int maxShotsPerLight=10000) 
		: Renderer(w,h,spp,s), mTracer(new MySimpleTracer(w,h,s,spp, numBounces, globalIllumination, globalPhotons, causticPhotons, volumePhotons)), mMaxShotsPerLight(maxShotsPerLight) {
		mTracer->setBackground(glm::vec3(0.f));
	}
	void render(const std::string &filename) {
		float exposure = -1.f;
		if(mTracer->isGlobalIllumination())
			mTracer->buildMaps(mMaxShotsPerLight);
		int pixeldone = 0;

		#pragma omp parallel for collapse(2)
		for (int y = 0; y < mHeight; y++) {
			//#pragma omp parallel for collapse(2)
			for (int x = 0; x < mWidth; x++) {
				glm::vec3 color;
				color = mTracer->calculatePixel(x,y);
				toneMapping(color, exposure);
				gammaCorrect(color);
				mImage[y][x] = glm::clamp(color, 0.f, 1.f);    
				#pragma omp critical 
				{
					pixeldone++;
					//printf("Pixel: %d %% \r", (pixeldone));
					printf("Progress: %6.2f %% rendered... \r", ((float)pixeldone / (mWidth*mHeight))*100.f);
				}
			}
			//printf("Progress: %6.2f %% rendered... \r", y*mWidth*100.f / (mWidth*mHeight));
		}
		mImage.writePPM(filename.c_str()); 
	}

	void toneMapping(glm::vec3 &color,  float exposure) {
		for(int i=0;i<3;i++) {
			color[i] = 1.0 - exp (color[i] * exposure);
		}
	}
    void gammaCorrect (glm::vec3 &color) {
		for(int i=0;i<3;i++) {
			if (color[i] <= 0.0031308f)
				color[i] *= 12.92f;
			else
				color[i] = 1.055f * pow (color[i], 0.4166667f) - 0.055f;
		}
	}
};

//class MySpectralRenderer : public Renderer {
//protected:
//	MySpectralTracer *mTracer;
//public:
//	MySpectralRenderer(int w, int h, int spp, Scene* s) : Renderer(w,h,spp,s), mTracer(new MySpectralTracer(w,h,s,spp)) {
//		mTracer->setBackground(glm::vec3(0.1f,0.3f,0.5f));
//	}
//	void render(const std::string &filename) {
//		for (int y = 0; y < mHeight; y++) {
//			for (int x = 0; x < mWidth; x++) {
//				mImage[y][x] = mTracer->calculatePixel(x,y); 
//			}
//			printf("Progress: %6.2f %% rendered... \r", y*mWidth*100.f / (mWidth*mHeight));
//		}
//		mImage.writePPM(filename.c_str()); 
//	}
//};

#endif 