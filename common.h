#ifndef COMMON_H_
#define COMMON_H_

#include <cstdint>
#include <memory>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <string>
#include <exception>
#include <stdexcept>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <vector>
#include <limits>
#include <chrono>

#include "vec.h"

using Clock = std::chrono::steady_clock;
constexpr float kInf = std::numeric_limits<float>::infinity();

#ifdef DEBUG
#define DBG(...) fprintf(stdout, __VA_ARGS__)
#else
#define DBG(...) {}
#endif

// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf  for C++11
template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
  int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
  if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
  auto size = static_cast<size_t>( size_s );
  auto buf = std::make_unique<char[]>( size );
  std::snprintf( buf.get(), size, format.c_str(), args ... );
  return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

// Width and height of canvas, and also the screen.
constexpr int kWidth = 600;
constexpr int kHeight = 600;
constexpr Color kBackgroundColor = {248, 240, 227};

// This gives us an FOV of about 53deg.
constexpr float kViewportWidth = 1.0f;
constexpr float kViewportHeight = 1.0f;

// Distance from the camera to the viewport.
constexpr float kD = 1;
constexpr Vec3<float> kCameraPos = {0, 0, 0};

// Converts canvas coordinates (from -C_w to +C_w-1) to screen coordinates
// (from 0 to S_w - 1)
// Ex: C = -3, -8 on 800,600 becomes S = 397, 308 (Note we flip Y)
inline Vec2<int> CanvasToScreen(const Vec2<int>& c) {
  return {(kWidth / 2) + c.x, (kHeight / 2) - c.y};
}

// Converts canvas coordinates (from -C_w to +C_w-1) to viewport coordinates
// (in scene space). Note that z is always equal to d.
inline Vec3<float> CanvasToViewport(const Vec2<int>& c) {
  return {c.x * (kViewportWidth / kWidth),
          c.y * (kViewportHeight / kHeight),
          kD};
}


// ToString()
inline std::string ToString(const Vec3<float>& v) {
  return string_format("(%f, %f, %f)", v.x, v.y, v.z);
}
inline std::string ToString(const Vec3<int>& v) {
  return string_format("(%d, %d, %d)", v.x, v.y, v.z);
}
inline std::string ToString(const Vec3<uint8_t>& v) {
  return string_format("(%d, %d, %d)", v.x, v.y, v.z);
}
inline std::string ToString(const Vec2<int>& v) {
  return string_format("(%d, %d)", v.x, v.y);
}
inline std::string ToString(const Vec2<float>& v) {
  return string_format("(%f, %f)", v.x, v.y);
}

#endif