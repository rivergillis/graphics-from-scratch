#ifndef VEC_H_
#define VEC_H_

#include <cstdlib>
#include <cmath>

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
  Vec3<T> operator*(const float amt) const {
    return {x * amt, y * amt, z * amt};
  }
  float Dot(const Vec3<T>& rhs) const {
    return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
  }
  float Length() const {
    const auto x_term = x * x;
    const auto y_term = y * y;
    const auto z_term = z * z;
    return sqrtf(x_term + y_term + z_term);
  }
  void Normalize() {
    const float length = Length();
    x /= length;
    y /= length;
    z /= length;
  }
};

using Color = Vec3<uint8_t>;
using Ray = Vec3<float>;

#endif  // VEC_H_
