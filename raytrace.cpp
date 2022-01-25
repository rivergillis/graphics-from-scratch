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
  const Ray obj_to_camera = camera_pos - nearest_sphere->center;
  const float illumination = ComputeLighting(scene, position, surface_normal, obj_to_camera, nearest_sphere->specular);
  return ColorMult(nearest_sphere->color, illumination);
}

float ComputeLighting(const Scene& scene, const Vec3<float>& position, const Ray& normal, const Ray& obj_to_camera, float specular) {
  // Sum up all of the intensities in the scene for this point
  float intensity = scene.ambient_intensity;
  for (const auto& light : scene.lights) {
    const Ray light_vec = light->Direction(position);
    const float n_dot_l = normal.Dot(light_vec);
    // Don't add negative values (this would illuminate the back of the surface)
    if (n_dot_l > 0) {
      intensity += light->Intensity() * n_dot_l / (normal.Length() * light_vec.Length());
    }

    // Specular lights (skip for matte objects).
    if (FloatEquals(specular, -1)) {
      continue;
    }

    const Ray reflection = (normal * 2 * normal.Dot(light_vec)) - light_vec;
    const float r_dot_v = reflection.Dot(obj_to_camera);
    if (r_dot_v > 0) {
      intensity += light->Intensity() * powf(r_dot_v / (reflection.Length() * obj_to_camera.Length()), specular);
    }
  }

  return intensity;
}
