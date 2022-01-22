#ifndef VEC_H_
#define VEC_H_

#include <cstdlib>

template <typename T>
struct Vec2 {
  T x;    // c
  T y;    // r

  Vec2<T> operator-(const Vec2<T>& rhs) const {
    return {x - rhs.x, y - rhs.y};
  }
  Vec2<T> operator+(const Vec2<T>& rhs) const {
    return {x + rhs.x, y + rhs.y};
  }
};

template <typename T>
struct Vec3 {
  T x;
  T y;
  T z;

  Vec3<T> operator-(const Vec3<T>& rhs) const {
    return {x - rhs.x, y - rhs.y, z - rhs.z};
  }
  Vec3<T> operator+(const Vec3<T>& rhs) const {
    return {x + rhs.x, y + rhs.y, z + rhs.z};
  }
  float Dot(const Vec3<T>& rhs) const {
    return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
  }
};

using Color = Vec3<uint8_t>;
using Ray = Vec3<float>;

#endif  // VEC_H_
