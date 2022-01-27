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
#include <sstream>
#include <filesystem>
#include <cstring>
#include <iostream>
#include <vector>
#include <limits>
#include <chrono>
#include <fstream>
#include <optional>

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
constexpr int kWidth = 900;
constexpr int kHeight = 900;
// constexpr Color kBackgroundColor = {248, 240, 227};
constexpr Color kBackgroundColor = {0, 0, 0};

// This gives us an FOV of about 53deg.
constexpr float kViewportWidth = 1.0f;
constexpr float kViewportHeight = 1.0f;

// Distance from the camera to the viewport.
constexpr float kD = 1;

// Converts canvas coordinates (from -C_w to +C_w-1) to screen coordinates
// (from 0 to S_w - 1)
// Ex: C = -3, -8 on 800,600 becomes S = 397, 308 (Note we flip Y)
inline Vec2<int> CanvasToScreen(const Vec2<int>& c) {
  return {(kWidth / 2) + c.x, (kHeight / 2) - c.y};
}

// Converts canvas coordinates (from -C_w to +C_w-1) to viewport coordinates
// (in scene space). Note that z is always equal to d.
// Note: this function causes the viewport to be places directly in front of
// the camera at a distance d.
inline Vec3<float> CanvasToViewport(const Vec2<int>& c) {
  return {c.x * (kViewportWidth / kWidth),
          c.y * (kViewportHeight / kHeight),
          kD};
}

// Saturation arithmetic for colors
// TODO: Verify that these are what's needed (not Color*Color)
inline uint8_t SatAdd(uint8_t lhs, uint8_t rhs) {
  const uint16_t big_res = static_cast<uint16_t>(lhs) + rhs;
  if (big_res > 0xFF) return 0xFF;
  return big_res;
}
inline uint8_t SatSub(uint8_t lhs, uint8_t rhs) {
  const int16_t big_res = static_cast<int16_t>(lhs) - rhs;
  if (big_res < 0) return 0;
  return big_res;
}
inline uint8_t SatMult(uint8_t lhs, uint8_t rhs) {
  const uint16_t big_res = static_cast<uint16_t>(lhs) * rhs;
  if (big_res > 0xFF) return 0xFF;
  return big_res;
}
inline uint8_t SatMult(uint8_t lhs, float rhs) {
  const float big_res = static_cast<float>(lhs) * rhs;
  if (big_res > 0xFF) return 0xFF;
  if (big_res < 0) return 0;
  return big_res;
}
inline Color ColorAdd(const Color& c, uint8_t amt) {
  return {SatAdd(c.x, amt), SatAdd(c.y, amt), SatAdd(c.z, amt)};
}  
inline Color ColorAdd(const Color& c1, const Color& c2) {
  return {SatAdd(c1.x, c2.x), SatAdd(c1.y, c2.y), SatAdd(c1.z, c2.z)};
}
inline Color ColorSub(const Color& c, uint8_t amt) {
  return {SatSub(c.x, amt), SatSub(c.y, amt), SatSub(c.z, amt)};
}
template <typename T>
inline Color ColorMult(const Color& c, T amt) {
  return {SatMult(c.x, amt), SatMult(c.y, amt), SatMult(c.z, amt)};
}

inline bool FloatEquals(float lhs, float rhs) {
static constexpr float kEpsilon = std::numeric_limits<float>::epsilon();
  return std::abs(lhs - rhs) < kEpsilon;
}

// Will overwrite the file if it exists.
inline void WriteFile(const std::string& filename, const std::string& contents) {
  std::ofstream out(filename);
  out << contents;
  out.close();
}

// Creates a subdirectory using the given string and returns the full path
// without adding any extra slashes.
inline std::string GetSubdirectory(const std::string& subdir) {
  std::string path = std::filesystem::current_path();
  path += "/" + subdir;
  if (!std::filesystem::is_directory(path) || !std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
}
  return path;
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