#ifndef SCENE_H_
#define SCENE_H_

#include "common.h"
#include "sphere.h"
#include "vec.h"

struct PointLight {
  float intensity;
  Vec3<float> position;

  Ray Direction(const Vec3<float> p) const {
    return position - p;
  }
};

struct DirectionalLight {
  float intensity;
  Ray direction;

  Ray Direction(const Vec3<float> p) const {
    return direction;
  }
};

struct Scene {
  std::vector<Sphere> spheres;

  float ambient_intensity;
  std::vector<PointLight> point_lights;
  std::vector<DirectionalLight> directional_lights;
};

#endif  // SCENE_H_