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
#include "Random.h"
#include "PhotonMap.h"
#include "Material.h"
#include "Light.h"
class  Tracer {
public:
	Tracer(int w, int h, Scene *s) : mWidth(w), mHeight(h), mScene(s) {}
	virtual ~Tracer() {}
	
	virtual glm::vec3 calculatePixel(int x, int y) const = 0;

protected:
	std::uint32_t mWidth;
	std::uint32_t mHeight;

	Scene *mScene;
};

class MySimpleTracer : public Tracer {
public:
	MySimpleTracer(int w, int h, Scene *s, int pixel_subdivs = 1, int max_bounces = 10, bool globalIllu = false, int globalPhotons = 10000, int causticPhotons = 10000, int volumePhotons = 10000)
		: Tracer(w, h, s), subdivs(pixel_subdivs), bounces(max_bounces), mBackground(0.0) , mGlobal(globalPhotons), mCaustics(causticPhotons), mVolume(volumePhotons), mGlobalIllumination(globalIllu){ 
		computeJitters();
	}
	virtual ~MySimpleTracer() {}

	void setBackground(const glm::vec3& color) { mBackground = color; }

	void incrementPixelSubdivs();
	void decrementPixelSubdivs();

	virtual glm::vec3 calculatePixel(int x, int y) const;

	void  buildMaps(unsigned int maxShotsPerLight = 10000) {
		const std::vector<Light*>& lights = mScene->getLights();
		if(lights.size() == 0)
			return;

		RandomFloat rnd;
	    for(int i = 0; i < lights.size(); ++i) {
			for(int j=0; j < maxShotsPerLight; j++) {
				Ray r;
				glm::vec3 energy;
				if(!lights[i]->emit(r, energy))
					continue;
				if(r.getHitMaterial()->mParticitatingMedium)
					traceParticleSSS(r, energy); //subsurface
				else
					traceParticle(r, energy); //global + caustic
			}
		}

		mGlobal.scale_photon_power(1.f/((float)lights.size()*maxShotsPerLight));
		mCaustics.scale_photon_power(1.f/((float)lights.size()*maxShotsPerLight));
		mVolume.scale_photon_power(1.f/((float)lights.size()*maxShotsPerLight));
		mGlobal.balance();
		mCaustics.balance();
		mVolume.balance();
		std::cout << "Global photonmap built with " << mGlobal.mapped_photons() << " stored photons" << std::endl;
		std::cout << "Caustic photonmap built with " << mCaustics.mapped_photons() << " stored photons" << std::endl;
		std::cout << "Volume photonmap built with " << mVolume.mapped_photons() << " stored photons" << std::endl;
	}

	glm::vec3 globalIradiance(const Ray &r, float mDistance, int particles) const {
		glm::vec3 result;
		mGlobal.irradiance_estimate(&result[0], &r.getHitPoint()[0], &r.hitNormal[0], mDistance, particles);
		return result;
	}
	glm::vec3 causticIradiance(const Ray &r, float mDistance, int particles) const {
		glm::vec3 result;
		mCaustics.irradiance_estimate(&result[0], &r.getHitPoint()[0], &r.hitNormal[0], mDistance, particles);
		return result;
	}
	glm::vec3 VollumeIradiance(const Ray &r, float mDistance, int particles) const {
		glm::vec3 result;
		mVolume.irradiance_estimate(&result[0], &r.getHitPoint()[0], &r.hitNormal[0], mDistance, particles);
		return result;
	}

	bool isGlobalIllumination() { return mGlobalIllumination; }
	void enableGlobalIllumination(bool g) { mGlobalIllumination = g;}

protected:
	void computeJitters();
	bool mGlobalIllumination;
	int subdivs, bounces;
	std::vector<glm::vec2> jitter;
	glm::vec2 win_to_vp;
	glm::vec2 lower_left;
	glm::vec2 step;
	glm::vec3 mBackground;

	PhotonMap mGlobal;
	PhotonMap mCaustics;
	PhotonMap mVolume;

	//test for g=0.4
	float henyesGreensteinPhaseFunction(float theta, float g) const { //probability for scattering
		RandomFloat rnd;
		return ((1.f-g*g) / (4*constants::pi * powf((1+g*g - 2*g*theta),1.5f)));
	}

	void traceParticleSSS(Ray r, glm::vec3 energy) {
		if(!r.getHitMaterial()->mParticitatingMedium)
			return;

		while(true) {

			if(r.recursiveDepth > bounces)
				return;

			
			float ps = r.getHitMaterial()->mScatterCoeff;
			float pa = r.getHitMaterial()->mAbsorptionCoeff;
			float pd = 1.0f - ps - pa; //color
			float extinction = r.getHitMaterial()->mScatterCoeff + r.getHitMaterial()->mAbsorptionCoeff;

			RandomFloat rnd;
			float prob = rnd();
			if(prob <= pd) { //diffuse reflexion
				glm::vec3 hitpoint = r.origin;
				energy *= r.getHitMaterial()->mDiffuse*pd;
				mVolume.store(&energy[0], &hitpoint[0], &r.dir[0]);
				return;
			} else if(prob <= ps + pd) { //just scattering
				float stepsize = -glm::log(rnd()/extinction);
				Sampling smp;
				glm::vec3 newDir = smp.sampleSphere();//isotropic
				float g=0.4;
				float costheta = (1.f/fabs(2*g))*(1+g*g-powf((1-g*g)/(1-g+2*g*rnd()),2.f)); //importance sampling
				float pdf = henyesGreensteinPhaseFunction(costheta, g);

				//olddir dot newdir / pdf?
				energy *= ps*glm::dot(r.dir, newDir)/pdf;
				Ray newRay;
				newRay.dir = glm::normalize(newDir);
				newRay.origin = r.getHitPoint();
			    newRay.recursiveDepth = r.recursiveDepth+1;
				newRay.t = stepsize;
				if(mScene->intersect(newRay)) {
					glm::vec3 normal = newRay.hitNormal;
					if(glm::dot(newRay.dir, normal) > 0) { // we are inside
						r.origin = newRay.getHitPoint();
						r.dir = glm::normalize(newDir);
						r.recursiveDepth = newRay.recursiveDepth;
					} else {//outside
						return;
					}
				} else {
					//we are already outside with the next step?
					return;
				}
			} else {
				//absorption
				return;
			}

		}
	}

	void traceParticle(Ray r, glm::vec3 energy/*Light* l, int maxGlobal, int maxCaustic*/) { //0=global, 1=caustic, 2=volume
	/*	Ray r;
	    glm::vec3 energy;
	    if(!l->emit(r, energy))
			return;*/
		
		bool caustic = false;
		while(true) {

			if(r.recursiveDepth > bounces)
				return;

			float pd = (r.getHitMaterial()->mDiffuse.x+r.getHitMaterial()->mDiffuse.y+r.getHitMaterial()->mDiffuse.z) /3.f;
			float ps = (r.getHitMaterial()->mSpecular.x+r.getHitMaterial()->mSpecular.y+r.getHitMaterial()->mSpecular.z) /3.f;
			float pt = (r.getHitMaterial()->mTransmission.x+r.getHitMaterial()->mTransmission.y+r.getHitMaterial()->mTransmission.z)/3.f;
			
			RandomFloat rnd;
			float prob = rnd();

			if(prob <= pd) { //diffuse reflexion
				glm::vec3 hitpoint = r.getHitPoint() + r.hitNormal*(float)0.0001f;
				energy *= r.getHitMaterial()->mDiffuse*pd;
				if(caustic) {//caustic
					for(int i = 0; i < 3; ++i){ //
						mCaustics.store(&energy[0], &hitpoint[0], &r.dir[0]);
					}
				} else {
					if(true) {
						mGlobal.store(&energy[0], &hitpoint[0], &r.dir[0]);
					}
				}
				//hemisphere cw sampling
				Sampling smp;
				Ray newRay;
				newRay.dir = smp.sampleCW(r.hitNormal);
				newRay.origin = hitpoint;
				caustic = false;
				newRay.recursiveDepth = r.recursiveDepth+1;
				if(!mScene->intersect(newRay)) {
					return;
				} else {
					r = newRay;
					continue;
				}
			} else if(prob <= pd+ps) { //specular reflexion
				Ray reflect;
				reflect.origin = r.getHitPoint() + (r.hitNormal * (float)0.0001f);
				reflect.dir = glm::normalize (glm::reflect (r.dir, r.hitNormal));
				reflect.recursiveDepth = r.recursiveDepth+1;
				caustic = true;
				energy *= r.getHitMaterial()->mSpecular*ps;
				if(mScene->intersect(reflect)) {
					r = reflect;
					continue;
				}
				else 
					return;
			} else if(prob <= pd+ps+pt) { //transmission
				float n, n2 = r.getHitMaterial()->ior, n1 = 1.00003f;
				glm::vec3 normal = r.hitNormal;
				if(glm::dot(r.dir, normal) > 0) { // we are inside
					normal = -normal;
					n2 = 1.00003f; // special ior
					n1 = r.getHitMaterial()->ior;
				}
				glm::vec3 new_dir;
				float kr = 0.0;
				float kt = 0.0;

				fresnelRefract(r, normal, n1, n2, new_dir, kr, kt);
				caustic = true;
				float anotherRandom = rnd();
				if(anotherRandom <= kr && false) { //reflexion
					energy*= ps*kr * r.getHitMaterial()->mSpecular;
					Ray reflect;
					reflect.origin = r.getHitPoint() + (r.hitNormal * (float)0.0001f);
					reflect.dir = glm::normalize (glm::reflect (r.dir, r.hitNormal));
					reflect.recursiveDepth = r.recursiveDepth+1;
					if(mScene->intersect(reflect)) {
						r = reflect;
						continue;
					}
					else 
						return;
				} else { //transmission
					energy*= pt*kt * r.getHitMaterial()->mTransmission;
					Ray refract;
					refract.origin = r.getHitPoint() + (new_dir * (float)0.001f);
					refract.dir = new_dir;
					refract.recursiveDepth = r.recursiveDepth+1;
					if(mScene->intersect(refract)) {
						r = refract;
						continue;
					}
					else 
						return;
				}
			} else { //absorption
				return;
			}
		}
	}

	glm::vec3 traceRay(Ray &ray, glm::vec3 multiplier) const {
		if(ray.recursiveDepth > bounces)
			return glm::vec3(0.f);

		glm::vec3 color(0.f);
		if(mScene->intersect(ray)) {
			if(ray.hitObject->isLight())
				return glm::vec3(0.9f);

			if(ray.recursiveDepth == 0) //add ambient color
				color = ray.getHitMaterial()->mAmbient;

			glm::vec3 diffuseColor = multiplier * ray.getHitMaterial()->mDiffuse;
			if (glm::dot (diffuseColor, diffuseColor) > 0.0001f) {
                color += diffuse (diffuseColor,ray);
            }
		                                // Specular
            glm::vec3 specularColor = multiplier * ray.getHitMaterial()->mSpecular;
			if (glm::dot (specularColor, specularColor) > 0.0001f) {
                color += specular(specularColor,ray);
            }

                                        // Transmissive.
			glm::vec3 transmissiveColor = multiplier * ray.getHitMaterial()->mTransmission;
			if (glm::dot (transmissiveColor, transmissiveColor) > 0.0001f) {
                color += transmission(transmissiveColor,ray);
            }
			
			if(ray.getHitMaterial()->mParticitatingMedium) {
				color += subsurfaceScattering(ray);
			}
			
		} else 
			return mBackground;

		return color;

	}

	glm::vec3 subsurfaceScattering(Ray r) const {
		if(r.recursiveDepth > bounces)
			return glm::vec3(0.f);

		Ray tmpRay;
		tmpRay.origin = r.getHitPoint() + tmpRay.dir*0.001f; 
		tmpRay.dir = r.dir;
		if(!mScene->intersect(tmpRay) || glm::dot(tmpRay.dir, tmpRay.hitNormal) <= 0) { // we are Outside
			return glm::vec3(0.f);
		}
			
		float extinction = r.getHitMaterial()->mScatterCoeff + r.getHitMaterial()->mAbsorptionCoeff;
		glm::vec3 color;

		RandomFloat rnd;
		float stepsize = -glm::log(rnd()/extinction);
		Sampling smp;
		glm::vec3 newDir = smp.sampleSphere();//isotropic

		Ray newRay;
		newRay.dir = glm::normalize(newDir);
		newRay.origin = r.getHitPoint();
		newRay.recursiveDepth = r.recursiveDepth+1;
		newRay.t = stepsize;
		newRay.hitObject = r.hitObject;
		newRay.hitNormal = r.hitNormal;

		color = VollumeIradiance(newRay, 0.3, 100);
		return color + subsurfaceScattering(newRay);
	}

	glm::vec3 diffuse(glm::vec3 diffuseColor, Ray ray) const {
		glm::vec3 result;
		float shadowFactor = 1.f;
		for (size_t i = 0; i < mScene->getLights().size(); ++i)  {
			Ray shadow;
			shadow.origin = ray.getHitPoint() + ray.hitNormal*(float)0.0001f; //hitpoint

			glm::vec3 intensity;
			
			if(!mScene->getLights()[i]->illuminate(shadow, intensity) /*&& glm::dot(shadow.getHitMaterial()->mTransmission, shadow.getHitMaterial()->mTransmission) < 0.0001*/)
				continue;//shadowFactor -= 0.8/(float)mScene->getLights().size();

			// Lambertian BRDF: http://seblagarde.wordpress.com/tag/lambertian-surface/
			glm::vec3 f = diffuseColor * 0.31830988618379067154f; 
      
			result += f * intensity * glm::max(glm::dot(ray.hitNormal, shadow.dir), 0.0f);
		}
		result *= shadowFactor;
		if(mGlobalIllumination) {
			result += (globalIradiance(ray,3.f, 350)+causticIradiance(ray,2.f, 350));
		}
		return result;
	}
	glm::vec3 specular(glm::vec3 specularColor, Ray ray) const {
        Ray spec;
		spec.origin = ray.getHitPoint() + (ray.hitNormal * (float)0.0001f);
		spec.dir = glm::normalize (glm::reflect (ray.dir, ray.hitNormal));
        //spec.origin = spec.origin;
		spec.recursiveDepth = ray.recursiveDepth+1;
        glm::vec3 result = traceRay (spec, specularColor);

        // Perform specular highlighting.
        for (size_t i = 0; i < mScene->getLights().size(); ++i)
        {
			Ray lightray;
			lightray.origin = ray.getHitPoint() + (ray.hitNormal * (float)0.0001f); //hitpoint

			glm::vec3 intensity;
			if(!mScene->getLights()[i]->illuminate(lightray, intensity))
				continue;
		
			glm::vec3 reflect = ray.dir - 2*glm::dot(ray.hitNormal,ray.dir)*ray.hitNormal;//glm::normalize(glm::reflect(ray.dir, ray.hitNormal));
			float cosLightReflect = glm::dot(glm::normalize(lightray.dir), reflect);

			if (cosLightReflect > 0) {
				glm::vec3 specularColor = glm::vec3(ray.getHitMaterial()->mSpecular);  
				float shininess = ray.getHitMaterial()->shininess;
					specularColor *= powf(cosLightReflect, shininess); 

				result += specularColor*intensity;
			}
        }

        return result;
	}

	void fresnelRefract(const Ray &ray, const glm::vec3 &normal, float n1, float n2, glm::vec3 &new_dir, float &kr, float &kt) const {
		float n = n1/n2;
		float cos_theta_i = fabs(glm::dot(-ray.dir, normal));
		float sin_theta_t_2 = (n*n) * (1.0 - (cos_theta_i * cos_theta_i));
		if (sin_theta_t_2 > (0.9999f)) {// Test for pure reflectance. 
                kr = 1.0f;
                kt = 0.0f;
        }
        else {
            float cos_theta_t = sqrtf (1.0 - sin_theta_t_2);
            float reflectance_ortho = (n1 * cos_theta_i - n2 * cos_theta_t) / (n1 * cos_theta_i + n2 * cos_theta_t);
            float reflectance_paral = (n2 * cos_theta_i - n1 * cos_theta_t) / (n2 * cos_theta_i + n1 * cos_theta_t);
            reflectance_ortho *= reflectance_ortho;
            reflectance_paral *= reflectance_paral;
            kr = 0.5 * (reflectance_ortho + reflectance_paral);
            kt = 1.0 - kr;
            new_dir = glm::normalize ((n * ray.dir) + normal * ((n * cos_theta_i) - cos_theta_t));
        }  
	}
	//Maybe implement subsurface scattering?!
	glm::vec3 transmission(glm::vec3 transmissiveColor, Ray ray) const {
        glm::vec3 result(0.f);
		
		float n, n2 = ray.getHitMaterial()->ior, n1 = 1.00003f;
		
		glm::vec3 normal = ray.hitNormal;
		if(glm::dot(ray.dir, normal) > 0) { // we are inside
			normal = -normal;
			n2 = 1.00003f; // special ior
			n1 = ray.getHitMaterial()->ior;
		}
		

        glm::vec3 new_dir;
        float kr = 0.0;
        float kt = 0.0;

		fresnelRefract(ray, normal, n1, n2, new_dir, kr, kt);

        if (kr > 0.0)  {
                result += specular (transmissiveColor * kr,  ray);
        }

        if (kt > 0.0) {
                Ray newRay;
				newRay.origin = ray.getHitPoint() + (new_dir * 0.0001f);
                newRay.dir = new_dir;
				newRay.recursiveDepth = ray.recursiveDepth+1;
                glm::vec3 tmpRes = traceRay (newRay, transmissiveColor * kt);

                // Apply Beer's Law.
				glm::vec3 absorbance = ray.getHitMaterial()->mDiffuse * 0.15f * -newRay.t;
                glm::vec3 transparency = glm::vec3 (expf (absorbance.r ), expf (absorbance.b ), expf (absorbance.g ));

                result += tmpRes * transparency;
        }
        return result;
	}

};

//class MySpectralTracer : public MySimpleTracer {
//public:
//	MySpectralTracer(int w, int h, Scene *s, int pixel_subdivs = 1)
//	: MySimpleTracer(w, h, s){}
//	virtual ~MySpectralTracer() {}
//
//	virtual glm::vec3 calculatePixel(int x, int y) const;
//	
//protected:
//private:
//	float estimateRadiance(const Ray &ray, float wavelength) const;
//};

//class MyPhotonTracer : public MySpectralTracer {
//public:
//	MyPhotonTracer(int w, int h, Scene *s, int pixel_subdivs = 1, int maxGlobal = 50000, int maxCaustics = 50000)
//	: MySpectralTracer(w, h, s, pixel_subdivs), mGlobal(maxGlobal), mCaustics(maxCaustics), mNoGlobals(maxGlobal), mNoCaustics(maxCaustics) { }
//	virtual ~MyPhotonTracer() {}
//
//	virtual glm::vec3 calculatePixel(int x, int y) const;
//	virtual glm::vec3 updatePixel(int x, int y) const;
//	
//protected:
//	PhotonMap mGlobal;
//	PhotonMap mCaustics;
//	int mNoGlobals, mNoCaustics;
//
//	float estimateCausticsIrradiance(const Ray& r, float maxDistance, int noParticles) const {
//		return mCaustics.irradiance_estimate(r.getHitPoint(), r.hitNormal, maxDistance, noParticles);
//	}
//    float estimateGlobalIrradiance(const Ray& r, float maxDistance, int noParticles) const{
//		return mGlobals.irradiance_estimate(r.getHitPoint(), r.hitNormal, maxDistance, noParticles);
//	}
//
//	void  buildMaps(unsigned int maxShots = 500000) {
//		const std::vector<Light*>& lights = mScene->getLights();
//		if(lights.size() == 0)
//			return;
//
//		RandomFloat rnd;
//	    for(int i = 0; i < mNoGlobals; ++i) {
//		  //random light source
//		  int index = static_cast<int>(lights.size()*rnd());
//		  while(index == lights.size())
//				index = static_cast<int>(lights.size()*rnd());
//
//		  //trace_particle(lights[light_idx], caustics_done, global_done);
//		}
//
//		mCaustics.scale_photon_power(lights.size()/static_cast<float>(mNoCaustics));
//	    mGlobal.scale_photon_power(lights.size()/static_cast<float>(mNoGlobals));
//	    mCaustics.balance();
//	    mGlobal.balance();
//	}
//	
//};

#endif
