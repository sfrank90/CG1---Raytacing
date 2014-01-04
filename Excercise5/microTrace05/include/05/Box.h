#pragma once
#ifndef BOX_H
#define BOX_H

#include "../Ray.h"

class Box
{
public:
  Vec3f min, max;

  Box()
  {
    clearBox();
  }

  /**
   * Initialize the sides of the box with -infinity
   * and +inifinity, respectively.
   */
  inline void clearBox()
  {
    min = Vec3f(INFINITY_RAY, INFINITY_RAY, INFINITY_RAY);
    max = Vec3f(-INFINITY_RAY, -INFINITY_RAY, -INFINITY_RAY);
  }

  /**
   * Extends the current box to hold in a point.
   */
  inline void extendBox(Vec3f a)
  {
    // IMPLEMENT ME 5.1a
    // Set the minimum and maximum so that a is inside.
	//if (a.x<min.x) min.x = a.x;
	//else if (a.x>max.x) max.x = a.x;
	//if (a.y<min.y) min.y = a.y;
	//else if (a.y>max.y) max.y = a.y;
	//if (a.z<min.z) min.z = a.z;
	//else if (a.z>max.z) max.z = a.z;
	// easier to read -> defined in vec3f
	min = minComponent(a, min);
	max = maxComponent(a, max);
    // IMPLEMENT ME END
  }

  /**
   * Extends the current box to hold in another box.
   */
  inline void extendBox(Box box)
  {
    extendBox(box.min);
    extendBox(box.max);
  }

  /**
   * Determines whether this box overlaps with another one.
   */
  inline bool overlapsBox(Box &b)
  {
    // IMPLEMENT ME 5.1a
    // Determine whether the boxes overlap.
	
	//if (max.x < b.min.x) return false; 
 //   if (min.x > b.max.x) return false; 
 //   if (max.y < b.min.y) return false; 
 //   if (min.y > b.max.y) return false; 
	//if (max.z < b.min.z) return false; 
 //   if (min.z > b.max.z) return false;
 //   return true; // boxes overlap
	for(int i = 0; i < 3; ++i)
		if(b.min[i] > max[i] || b.max[i] < min[i])
			return false;
	return true;
    // IMPLEMENT ME END
  }

  /**
   * Sets the near and far clipping point of a ray
   * with this box into t_near and t_far.
   */
  void clipBox(Ray &ray, float &tnear, float &tfar)
  {
    // IMPLEMENT ME 5.1a
    // Determine near and far point of the ray cutting the box.
	float tmin_x, tmax_x, tmin_y, tmax_y, tmin_z, tmax_z;

	tmin_x = (min.x - ray.org.x) / ray.dir.x;
	tmax_x = (max.x - ray.org.x) / ray.dir.x;

	tmin_y = (min.y - ray.org.y) / ray.dir.y;
	tmax_y = (max.y - ray.org.y) / ray.dir.y;

	tmin_z = (min.z - ray.org.z) / ray.dir.z;
	tmax_z = (max.z - ray.org.z) / ray.dir.z;

	// get real in and out for tx ty and tz
	float tin_x, tout_x, tin_y, tout_y, tin_z, tout_z;

	tin_x = min(tmin_x, tmax_x);
	tout_x = max(tmin_x, tmax_x);

	tin_y = min(tmin_y, tmax_y);
	tout_y = max(tmin_y, tmax_y);

	tin_z = min(tmin_z, tmax_z);
	tout_z = max(tmin_z, tmax_z);

	//find global entry and exit
	float in_temp = min(tin_x, tin_y);
	tnear = min(in_temp, tin_z); //global entry 
	float out_temp = max(tin_x, tin_y);
	tfar = max(out_temp, tout_z); //global exit 

    // IMPLEMENT ME END
  }

};

#endif
