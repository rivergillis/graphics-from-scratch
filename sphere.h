#ifndef SPHERE_H_
#define SPHERE_H_

#include "common.h"
#include "vec.h"

struct Sphere {
  Vec3<float> center;
  float radius;
  Color color;

  // returns infinity for no solution.
  Vec2<float> IntersectRay(const Vec3<float>& camera_pos, const Ray& ray) const {
    // const Ray to_camera = RayTo(camera_pos, center);  // CO = O - C
    const Ray to_camera = camera_pos - center;

    // Solve for a ray-sphere intersection.
    const float a = ray.Dot(ray);
    const float b = 2 * to_camera.Dot(ray);
    const float c = to_camera.Dot(to_camera) - (radius * radius);
    const float discriminant = b*b - 4*a*c;
    std::cout << string_format("ray (D) %s  to_camera (CO) %s discrim %f\n", ToString(ray).c_str(), ToString(to_camera).c_str(), discriminant );
    if (discriminant < 0) {
      return {kInf, kInf};
    }

    // D dot D = 1.284220881389
    // 

// x 399 y 112 color (248, 240, 227) Screen xy (799, 188)
// ray (D) (0.498750, 0.188333, 1.000000)  camera_to_us (CO) (2.000000, 0.000000, 4.000000) discrim 2.299232
    // D: forward thru z, x to right, y up. D looks right.
    // CO: we are at 2,0,4 so this is right.
// RET t (-3.301098, -4.481831) a 1.284221 b 9.995000 c 19.000000
    // okay so the values for t are fine given the a,b,c input...
    // but we just need to figure out why a,b,c give us negative roots.
    const Vec2<float> t = {
      (-b + sqrt(discriminant)) / (2*a),
      (-b - sqrt(discriminant)) / (2*a)
    };
    std::cout << string_format("RET t %s a %f b %f c %f\n", ToString(t).c_str(), a, b, c);
    return t;
  }
};

#endif  // SPHERE_H_
