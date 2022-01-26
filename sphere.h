#ifndef SPHERE_H_
#define SPHERE_H_

#include "common.h"
#include "vec.h"

struct Sphere {
  Vec3<float> center;
  float radius;
  Color color;
  float specular; // -1 is matte, 10 is a little shiny, 1000 is very shiny.
  float reflective; // 0 to 1

  // returns infinity for no solution.
  Vec2<float> IntersectRay(const Vec3<float>& camera_pos, const Ray& ray) const {
    const Ray to_camera = camera_pos - center;

    // Solve for a ray-sphere intersection.
    const float a = ray.Dot(ray);
    const float b = 2 * to_camera.Dot(ray);
    const float c = to_camera.Dot(to_camera) - (radius * radius);
    const float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
      return {kInf, kInf};
    }

    const Vec2<float> t = {
      (-b + sqrt(discriminant)) / (2*a),
      (-b - sqrt(discriminant)) / (2*a)
    };
    return t;
  }

  Ray Normal(const Vec3<float>& position) const {
    Ray n = position - center;
    n.Normalize();
    return n;
  }
};

#endif  // SPHERE_H_
