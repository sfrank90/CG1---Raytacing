#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Primitive.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"

/**
 * NONORMALS: OBJ-File without normals
 * NORMALS: OBJ-File with normals
 * TEXTURENORMALS: OBJ-File with texture-coordinates and normals
 */
enum OBJStyle
{
  NONORMALS, NORMALS, TEXTURENORMALS
};

/**
 * SMOOTH: use SmoothTriangles
 * STANDARD: use standard Triangles
 */
enum TriangleStyle
{
  SMOOTH, STANDARD
};

class Texture;
// forward declaration

// ----------------------------------------------------------------------------

class Scene
{
public:
  Vec3f bgColor;                      // background color
  std::vector<Primitive *> primitive; // primitives
  std::vector<Light *> lights;        // lights
  std::vector<Texture*> textures;     // textures
  Camera *camera;                     // camera (with or without DoF)

  Scene(int resx, int resy)
      : bgColor(Vec3f(0, 0, 0)), camera(NULL)
  {
  }

  virtual ~Scene()
  {
    // Delete primitives.
    for (vector<Primitive*>::iterator it = primitive.begin(); it != primitive.end(); ++it)
    {
      delete *it;
    }

    // Delete lights.
    for (vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
    {
      delete *it;
    }

    // Delete textures.
#if ENABLE_TEXTURES == true
    {
      for (vector<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
      {
        delete *it;
      }
    }
#endif

  }

  // --------------------------------------------------------------------------

  /**
   * Creates a triangle (standard, smooth or textured).
   * - tri: coords.
   * - norm: normals.
   * - tex: texture coords.
   */
  virtual Primitive* createTrianglePrimitive(Shader* shd, TriangleStyle tstyle, OBJStyle objstyle, // shader, triangle style, object style.
      Vec3f* tri, Vec3f* norm, Vec3f* tex) = 0;

  // --------------------------------------------------------------------------

  /**
   * Adds a new primitive.
   */
  void add(Primitive *prim)
  {
    primitive.push_back(prim);
  }

  /**
   * Adds a new light.
   */
  void add(Light *light)
  {
    lights.push_back(light);
  }

#if ENABLE_TEXTURES == true
  /**
   * Adds a new texture.
   */
  void add(Texture* t)
  {
    textures.push_back(t);
  }
#endif

  // --------------------------------------------------------------------------

  /**
   * findIntersection all contained objects.
   */
  bool findIntersection(Ray &ray)
  {
    bool hit = false;
    for (unsigned int i = 0; i < primitive.size(); i++)
    {
      hit |= primitive.at(i)->findIntersection(ray);
    }
    return hit;
  }

  /**
   * Find occluder.
   */
  bool isOccluded(Ray &ray)
  {
    for (unsigned int i = 0; i < primitive.size(); i++)
    {
      if (primitive.at(i)->findIntersection(ray))
      {
        return true;
      }
    }
    return false;
  }

  // --------------------------------------------------------------------------

  /**
   * Trace the given ray, shade it and return the color of the shaded ray.
   */
  Vec3f rayTrace(Ray &ray)
  {
    if (findIntersection(ray) && (++ray.recursionLevel < MAX_RECURSION_LEVEL))
    {
      if (ray.hit->shader == NULL)
      {
        cerr << "Warning: Primitive[" << ray.hit->getClassName() << "] has no shader." << endl;
        exit(1);
        return bgColor;
      }
      else
      {
        return ray.hit->shader->shadeRay(ray);
      }
    }
    else
    {
      return bgColor;
    }
  }

  // --------------------------------------------------------------------------

  /**
   * Read in an obj file. Only triangles are supported.
   */
  void parseOBJ(const char *fileName, Shader* shd, OBJStyle objstyle, TriangleStyle tstyle, int factor)
  {
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
      cout << "(Scene): Could not open OBJ file: " << fileName << endl;
      return;
    }
    else
    {
      cout << "(Scene): loaded OBJ file: " << fileName << endl;
    }

    char line[1000];

    vector<int> faces;
    vector<int> normalsID;
    vector<int> texturesID;

    vector<float> vert;
    vector<float> normals;
    vector<float> texus;
    vector<float> texvs;

    // Read vertices, normals, textures, and faces.
    while (fgets(line, 1000, file) && !feof(file))
    {
      // Vertices.
      if (strncmp(line, "v ", 2) == 0)
      {
        float a, b, c;
        sscanf(line, "v %f %f %f", &a, &b, &c);

        vert.push_back(a);
        vert.push_back(b);
        vert.push_back(c);
      }

      // Normals.
      if (strncmp(line, "vn ", 3) == 0)
      {
        float a, b, c;
        sscanf(line, "vn %f %f %f", &a, &b, &c);

        normals.push_back(a);
        normals.push_back(b);
        normals.push_back(c);
      }

      // Texture vertices.
      if (strncmp(line, "vt ", 2) == 0)
      {
        float a, b;
        sscanf(line, "vt %f %f", &a, &b);

        texus.push_back(a);
        texvs.push_back(b);
      }

      // Faces.
      if (strncmp(line, "f ", 2) == 0)
      {
        int a, b, c;
        int a1, b1, c1;
        int a2, b2, c2;

        if (objstyle == TEXTURENORMALS)
        {
          sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &a1, &a2, &b, &b1, &b2, &c, &c1, &c2);
          texturesID.push_back(a1);
          texturesID.push_back(b1);
          texturesID.push_back(c1);

          normalsID.push_back(a2);
          normalsID.push_back(b2);
          normalsID.push_back(c2);
        }
        else if (objstyle == NORMALS)
        {
          sscanf(line, "f %d/%d %d/%d %d/%d", &a, &a1, &b, &b1, &c, &c1);
          normalsID.push_back(a1);
          normalsID.push_back(b1);
          normalsID.push_back(c1);
        }
        else if (objstyle == NONORMALS)
        {
          sscanf(line, "f %d %d %d", &a, &b, &c); // Only triangles given.
        }
        else
        {
          printf("(Scene): OBJStyle unknown. exiting...\n");
          exit(1);
        }

        faces.push_back(a);
        faces.push_back(b);
        faces.push_back(c);
      }
    }

    // For each face, add the corresponding triangle primitive.
    for (unsigned int n = 0; n < faces.size() / 3; n++)
    {
      // Vertex positions of triangle; normals, texture coords.
      Vec3f tri[3];
      Vec3f norm[3];
      Vec3f tex[3];

      // Index into face list, normal list.
      int vertID[3];
      int normID[3];

      vertID[0] = faces[n * 3 + 0] - 1;
      vertID[1] = faces[n * 3 + 1] - 1;
      vertID[2] = faces[n * 3 + 2] - 1;

      // Any kind of normals?
      if ((objstyle == NORMALS) || (objstyle == TEXTURENORMALS))
      {
        normID[0] = normalsID[n * 3 + 0] - 1;
        normID[1] = normalsID[n * 3 + 1] - 1;
        normID[2] = normalsID[n * 3 + 2] - 1;
      }

      tri[0][0] = vert[vertID[0] * 3 + 0] * factor;
      tri[0][1] = vert[vertID[0] * 3 + 1] * factor;
      tri[0][2] = vert[vertID[0] * 3 + 2] * factor;

      tri[1][0] = vert[vertID[1] * 3 + 0] * factor;
      tri[1][1] = vert[vertID[1] * 3 + 1] * factor;
      tri[1][2] = vert[vertID[1] * 3 + 2] * factor;

      tri[2][0] = vert[vertID[2] * 3 + 0] * factor;
      tri[2][1] = vert[vertID[2] * 3 + 1] * factor;
      tri[2][2] = vert[vertID[2] * 3 + 2] * factor;

      // Any kind of normals?
      if ((objstyle == NORMALS) || (objstyle == TEXTURENORMALS))
      {
        norm[0][0] = normals[normID[0] * 3 + 0];
        norm[0][1] = normals[normID[0] * 3 + 1];
        norm[0][2] = normals[normID[0] * 3 + 2];

        norm[1][0] = normals[normID[1] * 3 + 0];
        norm[1][1] = normals[normID[1] * 3 + 1];
        norm[1][2] = normals[normID[1] * 3 + 2];

        norm[2][0] = normals[normID[2] * 3 + 0];
        norm[2][1] = normals[normID[2] * 3 + 1];
        norm[2][2] = normals[normID[2] * 3 + 2];
      }

      // Smooth triangle with texture?
      if ((objstyle == TEXTURENORMALS) && (tstyle == SMOOTH))
      {
        // Set current texture coordinates for triangle by performing lookup
        // in texus and texvs with the texturesID from the face.
        // (note: the third entry of tex is zero, i.e. tex[ ... ][2] = 0)
        tex[0][0] = texus[texturesID[n * 3 + 0] - 1];
        tex[1][0] = texus[texturesID[n * 3 + 1] - 1];
        tex[2][0] = texus[texturesID[n * 3 + 2] - 1];
        tex[0][1] = texvs[texturesID[n * 3 + 0] - 1];
        tex[1][1] = texvs[texturesID[n * 3 + 1] - 1];
        tex[2][1] = texvs[texturesID[n * 3 + 2] - 1];
        tex[0][2] = 0;
        tex[1][2] = 0;
        tex[2][2] = 0;
      }

      // Use virtual method to determine the type of triangle added.
      add(createTrianglePrimitive(shd, tstyle, objstyle, tri, norm, tex));
    }

    cout << "(Scene): Number of vertices : " << (vert.size() / 3) << endl;
    cout << "(Scene): Number of triangles: " << primitive.size() << endl;
  }

};

#endif
