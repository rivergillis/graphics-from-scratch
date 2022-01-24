#include "raytrace.h"

#include "common.h"
#include "scene.h"

Color TraceRay(const Scene& scene, const Vec3<float>& camera_pos, const Ray& ray, float t_min, float t_max) {
  // Find the sphere intersection closest to us, then just use that sphere's color.
  const Sphere* nearest_sphere = nullptr;
  float nearest_t = kInf;
  for (const auto& sphere : scene.spheres) {
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

  const Vec3<float> position = ray * nearest_t;
  const Ray surface_normal = nearest_sphere->Normal(position);
  return ColorMult(nearest_sphere->color, ComputeLighting(scene, position, surface_normal));
}

float ComputeLighting(const Scene& scene, const Vec3<float>& position, const Ray& normal) {
  // Sum up all of the intensities in the scene for this point
  float intensity = scene.ambient_intensity;
  for (const auto& point_light : scene.point_lights) {
    const Ray light_vec = point_light.Direction(position);
    const float n_dot_l = normal.Dot(light_vec);
    // Don't add negative values (this would illuminate the back of the surface)
    if (n_dot_l > 0) {
      intensity += point_light.intensity * n_dot_l / (normal.Length() * light_vec.Length());
    }
  }

  // Do the same thing for directional lights.
  for (const auto& directional_light : scene.directional_lights) {
    const Ray light_vec = directional_light.Direction(position);
    const float n_dot_l = normal.Dot(light_vec);
    // Don't add negative values (this would illuminate the back of the surface)
    if (n_dot_l > 0) {
      intensity += directional_light.intensity * n_dot_l / (normal.Length() * light_vec.Length());
    }
  }

  return intensity;
}
