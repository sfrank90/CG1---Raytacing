#pragma once
#ifndef VEC3F_H
#define VEC3F_H

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <stdlib.h>

#ifdef WIN32
#include <string>
#else
#include <string.h>
#include <unistd.h>
#endif

#include <vector>
#include <set>
#include <math.h>
#include <assert.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#define max(a,b) MAX(a,b)
#define min(a,b) MIN(a,b)

#define EPSILON 1E-4
#define INFINITY_RAY HUGE_VAL

using namespace std;

class Vec3f
{
public:
  float x, y, z;

  /*! Initializes an all-zero vector. */
  Vec3f()
      : x(0), y(0), z(0)
  {
  }

  /*! Initializes a same-value vector. */
  Vec3f(float f)
      : x(f), y(f), z(f)
  {
  }

  /*! Initializes a new 3-vector. */
  Vec3f(float x, float y, float z)
      : x(x), y(y), z(z)
  {
  }

  /*! Resets all values to 0. */
  inline void clear()
  {
    x = 0;
    y = 0;
    z = 0;
  }

  /*! Returns the i'th component of the vector. */
  inline const float &operator[](const int i) const
  {
    return *(&x + i);
  }

  /*! Returns the i'th component of the vector. */
  inline float &operator[](const int i)
  {
    return *(&x + i);
  }

  /*! Copies the values from another vector. */
  inline Vec3f &operator=(const Vec3f &b)
  {
    x = b.x;
    y = b.y;
    z = b.z;
    return *this;
  }

  /*! Returns the dimension with the largest size. */
  inline int maxDim() const
  {
    return (x > y) ? ((x > z) ? 0 : 2) : ((y > z) ? 1 : 2);
  }

  /*! Sets the minimum of this and a second vector, component-wise. */
  inline void setminComponent(Vec3f &other)
  {
    x = min(x, other.x);
    y = min(y, other.y);
    z = min(z, other.z);
  }

  /*! Sets the maximum of this and a second vector, component-wise. */
  inline void setmaxComponent(Vec3f &other)
  {
    x = max(x, other.x);
    y = max(y, other.y);
    z = max(z, other.z);
  }

};

/*! Same as dot product. */
inline float operator*(const Vec3f &a, const Vec3f &b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

/*! Dot product. */
inline float dot(const Vec3f &a, const Vec3f &b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

/*! Single-variable component-wise product. */
inline Vec3f componentProduct(const Vec3f &a, const Vec3f &b)
{
  return Vec3f(a.x * b.x, a.y * b.y, a.z * b.z);
}

/*! Cross (perpendicularity) product. */
inline Vec3f cross(const Vec3f &a, const Vec3f &b)
{
  return Vec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

/*! Same as cross product. */
inline Vec3f operator^(const Vec3f &a, const Vec3f &b)
{
  return Vec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

/*! Negative of each component. */
inline Vec3f operator-(const Vec3f &v)
{
  return Vec3f(-v.x, -v.y, -v.z);
}

/*! Returns the length of the vector. */
inline float length(const Vec3f &v)
{
  return sqrt(dot(v, v));
}

/*! Multiply each component of a Vec3f with a float. */
inline Vec3f operator*(const float f, const Vec3f &v)
{
  return Vec3f(f * v.x, f * v.y, f * v.z);
}

/*! Multiply each component of a Vec3f with a float. */
inline Vec3f operator*(const Vec3f &v, const float f)
{
  return Vec3f(f * v.x, f * v.y, f * v.z);
}

/*! Assign each component of a Vec3f with a float. */
inline void operator*=(Vec3f &v, const float f)
{
  v.x *= f;
  v.y *= f;
  v.z *= f;
}

/*! Multiply each component of a Vec3f with the corresponding component of a second Vec3f. */
inline void operator*=(Vec3f &v, const Vec3f &f)
{
  v.x *= f.x;
  v.y *= f.y;
  v.z *= f.z;
}

/*! Divide each component of a Vec3f by a float, */
inline Vec3f operator/(const Vec3f &v, const float f)
{
  return (1 / f) * v;
}

/*! Divide each component of a Vec3f by a float. */
inline void operator/=(Vec3f &v, const float f)
{
  v *= (1 / f);
}

/*! Add one Vec3f to another. */
inline Vec3f operator+(const Vec3f &a, const Vec3f &b)
{
  return Vec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

/*! Add one Vec3f to another. */
inline Vec3f &operator+=(Vec3f &a, const Vec3f &b)
{
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}

/*! Difference between two Vec3f. */
inline Vec3f operator-(const Vec3f &a, const Vec3f &b)
{
  return Vec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}

/*! Normalize a Vec3f (length of vector will be 1). */
inline void normalize(Vec3f &v)
{
  v *= (1. / length(v));
}

/*! Print a Vec3f. */
inline ostream &operator<<(ostream &o, const Vec3f &v)
{
  o << "(" << v.x << "," << v.y << "," << v.z << ")";
  return o;
}

/*! Return the minimum of two Vec3f. */
inline Vec3f minComponent(const Vec3f a, const Vec3f b)
{
  return Vec3f(MIN(a.x, b.x), MIN(a.y, b.y), MIN(a.z, b.z));
}

/*! Return the maximum of two Vec3f. */
inline Vec3f maxComponent(const Vec3f a, const Vec3f b)
{
  return Vec3f(MAX(a.x, b.x), MAX(a.y, b.y), MAX(a.z, b.z));
}

/*! Return the clamped version of a Vec3f. */
inline Vec3f clampComponent(const Vec3f a, float min_val, float max_val)
{
  return Vec3f(MIN(MAX(a.x, min_val), max_val), MIN(MAX(a.y, min_val), max_val), MIN(MAX(a.z, min_val), max_val));
}

#endif
