#ifndef RAYTRACE_H_
#define RAYTRACE_H_

#include "vec.h"
#include "sphere.h"
#include "common.h"
#include "scene.h"

using Intersection = std::pair<float, Sphere>;
std::optional<Intersection> ClosestIntersection(const Scene& scene, const Ray& ray, const Vec3<float>& ray_start, float t_min, float t_max);

// Traces a ray from t_min to t_max, returns the color for the canvas.
Color TraceRay(const Scene& scene, const Vec3<float>& ray_start, const Ray& ray, float t_min, float t_max, int recursion_depth);

// Computes the light intensity for a position and surface normal in the scene.
float ComputeLighting(const Scene& scene, const Vec3<float>& position, const Ray& normal, const Ray& obj_to_camera, float specular);

#endif  // RAYTRACE_H_