/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*
* TODO: Hit object and hit material...
*
*/

#include "../include/Random.h"
#include "../include/Tracer.h"

void MyTracer::computeJitters() {
	RandomFloat rnd;

	float aspect = static_cast<float>(mWidth)/static_cast<float>(mHeight);
	win_to_vp[0] = aspect/static_cast<float>(mWidth);
	win_to_vp[1] = 1.0f/static_cast<float>(mHeight);
	lower_left = glm::vec2(aspect*(-0.5f), -0.5f);
	step = win_to_vp/static_cast<float>(subdivs);

	jitter.resize(subdivs*subdivs, glm::vec2(0.5f)*win_to_vp);

	for(unsigned int i = 0; i < subdivs; ++i)
		for(unsigned int j = 1; j < subdivs; ++j) 
			jitter[i*subdivs + j] = glm::vec2(rnd() + j, rnd() + i)*step; 
}

void MyTracer::incrementPixelSubdivs() {
	++subdivs;
	computeJitters();
}

void MyTracer::decrementPixelSubdivs() {
	if(subdivs > 1) {
		--subdivs;
		computeJitters();
	}
}

glm::vec3 MyTracer::calculatePixel(int x, int y) const {
  glm::vec3 result(0.0);
  glm::vec2 vp_pos = glm::vec2(x, y)*win_to_vp + lower_left;

  for(unsigned int i = 0; i < jitter.size(); ++i) {
	  Ray ray;
	  if(mScene->dof()) {
		  float angle = (jitter[i].x/win_to_vp.x) * constants::two_pi; 
          float radius = 0.5*jitter[i].y; 
          glm::vec2 offset(std::cos(angle) * radius, std::sin(angle) * radius);
		  mScene->getCamera()->initRay(vp_pos.x + 0.5f*win_to_vp.x,  vp_pos.y + 0.5f*win_to_vp.y, offset.x, offset.y, ray);
	  }
	  else {
		  mScene->getCamera()->initRay(vp_pos.x + jitter[i].x, vp_pos.y + jitter[i].y, ray);
	  }
	  
	  if(mScene->intersect(ray)) {
		  //ray.hasHit = true;
		  //const Shader *s = mScene->getShader(ray);
		  //if(s)
	      // result += s->shade(ray);
		  result += glm::vec3(0.2); //dummy for the moment
	  } else {
		  result += mBackground;
	  }
  }
  return result/static_cast<float>(jitter.size());
}

//const Shader *RayCaster::getShader(const geometry::Ray &ray) const {
//	return mScene->getShader(ray);
//}
