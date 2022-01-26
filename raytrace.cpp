#include "raytrace.h"

#include "common.h"
#include "scene.h"

namespace {
  Ray ReflectRay(const Ray& ray, const Vec3<float>& normal) {
    return (normal * 2 * normal.Dot(ray)) - ray;
  }
} // namespace

std::optional<Intersection> ClosestIntersection(const Scene& scene, const Ray& ray, const Vec3<float>& ray_start, float t_min, float t_max) {
  const Sphere* nearest_sphere = nullptr;
  float nearest_t = kInf;
  for (const auto& sphere : scene.spheres) {
    const Vec2<float> t = sphere.IntersectRay(ray_start, ray);
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
    return std::nullopt;
  }
  return std::optional<Intersection>{Intersection{nearest_t, *nearest_sphere}};
}

Color TraceRay(const Scene& scene, const Vec3<float>& ray_start, const Ray& ray, float t_min, float t_max, int recursion_depth) {
  const auto intersection = ClosestIntersection(scene, ray, ray_start, t_min, t_max);
  if (!intersection) { return kBackgroundColor; }

  // Get the local color.
  const Vec3<float> position = ray_start + ray * intersection->first;
  const Ray surface_normal = intersection->second.Normal(position);
  const float illumination = ComputeLighting(scene, position, surface_normal, -ray, intersection->second.specular);
  const Color local_color = ColorMult(intersection->second.color, illumination);

  // Compute the reflected color until we can't or should stop.
  const float reflective = intersection->second.reflective;
  if (recursion_depth <= 0 || reflective <= 0) {
    return local_color;
  }
  const Ray reflected_ray = ReflectRay(-ray, surface_normal);
  const Color reflected_color = TraceRay(scene, position, reflected_ray, 0.1f, kInf, recursion_depth - 1);
  // return reflected_color;
  return ColorAdd(ColorMult(local_color, (1 - reflective)), ColorMult(reflected_color, reflective));
}

float ComputeLighting(const Scene& scene, const Vec3<float>& position, const Ray& normal, const Ray& obj_to_camera, float specular) {
  // Sum up all of the intensities in the scene for this point
  float intensity = scene.ambient_intensity;
  for (const auto& light : scene.lights) {
    const Ray light_vec = light->Direction(position);
    // First check if we're in the shadow
    if (ClosestIntersection(scene, light_vec, position, 0.001f, light->TMax())) {
      continue;
    }

    // Diffuse lights
    const float n_dot_l = normal.Dot(light_vec);
    if (n_dot_l > 0) {
      intensity += light->Intensity() * n_dot_l / (normal.Length() * light_vec.Length());
    }

    // Specular lights (skip for matte objects).
    if (FloatEquals(specular, -1)) {
      continue;
    }

    const Ray reflection = ReflectRay(light_vec, normal);
    const float r_dot_v = reflection.Dot(obj_to_camera);
    if (r_dot_v > 0) {
      intensity += light->Intensity() * powf(r_dot_v / (reflection.Length() * obj_to_camera.Length()), specular);
    }
  }

  return intensity;
}
