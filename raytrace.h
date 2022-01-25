#ifndef RAYTRACE_H_
#define RAYTRACE_H_

#include "vec.h"
#include "sphere.h"
#include "common.h"
#include "scene.h"

// Traces a ray from t_min to t_max, returns the color for the canvas.
Color TraceRay(const Scene& scene, const Vec3<float>& camera_pos, const Ray& ray, float t_min, float t_max);

// Computes the light intensity for a position and surface normal in the scene.
float ComputeLighting(const Scene& scene, const Vec3<float>& position, const Ray& normal, float specular);

#endif  // RAYTRACE_H_