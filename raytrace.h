#ifndef RAYTRACE_H_
#define RAYTRACE_H_

#include "vec.h"
#include "sphere.h"
#include "common.h"

// Traces a ray from t_min to t_max, returns the color for the canvas.
Color TraceRay(const std::vector<Sphere>& scene, const Vec3<float>& camera_pos, const Ray& ray, float t_min, float t_max);

#endif  // RAYTRACE_H_