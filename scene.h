#ifndef SCENE_H_
#define SCENE_H_

#include "common.h"
#include "sphere.h"
#include "vec.h"

struct Light {
  virtual Ray Direction(const Vec3<float> pos) const = 0;
  virtual float Intensity() const = 0;
  virtual float TMax() const = 0; // for shadows
  virtual ~Light() {}
};

struct PointLight : Light {
  Vec3<float> position_;
  float intensity_;
  PointLight(const Vec3<float>& position, float intensity) {
    position_ = position;
    intensity_ = intensity;
  }
  Ray Direction(const Vec3<float> p) const override {
    return position_ - p;
  }
  float Intensity() const override { return intensity_; }
  float TMax() const override { return 1; }
};

struct DirectionalLight : Light {
  Ray direction_;
  float intensity_;
  DirectionalLight(const Ray& direction, float intensity) {
    direction_ = direction;
    intensity_ = intensity;
  }
  Ray Direction(const Vec3<float> p) const override {
    return direction_;
  }
  float Intensity() const override { return intensity_; }
  float TMax() const override { return kInf; }
};

struct Scene {
  std::vector<Sphere> spheres;

  float ambient_intensity;
  std::vector<std::unique_ptr<Light>> lights;
};

#endif  // SCENE_H_