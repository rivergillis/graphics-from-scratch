#include "raytrace.h"

#include "common.h"

Color TraceRay(const std::vector<Sphere>& scene, const Vec3<float>& camera_pos, const Ray& ray, float t_min, float t_max) {
  // Find the sphere intersection closest to us, then just use that sphere's color.
  const Sphere* nearest_sphere = nullptr;
  float nearest_t = kInf;
  for (const auto& sphere : scene) {
    const Vec2<float> t = sphere.IntersectRay(camera_pos, ray);
    if (t.x >= t_min && t.x < t_max && t.x < nearest_t) {
      nearest_t = t.x;
      nearest_sphere = &sphere;
    }

    if (t.y >= t_min && t.y < t_max && t.y < nearest_t) {
      nearest_t = t.y;
      nearest_sphere = &sphere;
    }
  }

  if (nearest_sphere == nullptr) {
    return kBackgroundColor;
  }
  return nearest_sphere->color;
}
