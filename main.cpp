#include "common.h"
#include "sdl_viewer.h"
#include "image.h"
#include "vec.h"
#include "sphere.h"
#include "raytrace.h"
#include "scene.h"
#include "mat.h"

constexpr int kRecursionDepth = 5;

void SetImage(Image* img) {
  Scene scene;
  scene.spheres.push_back({.center = {0, -1, 3}, .radius = 1, .color = {255, 0, 0}, .specular = 500, .reflective = 0.2});
  scene.spheres.push_back({.center = {2, 0, 4}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene.spheres.push_back({.center = {-2, 0, 4}, .radius = 1, .color = {0, 255, 0}, .specular = 10, .reflective = 0.4});
  scene.spheres.push_back({.center = {0, -5001, 0}, .radius = 5000, .color = {255, 255, 0}, .specular = 1000, .reflective = 0.5});
  // scene.spheres.push_back({.center = {0, 0.7, 2}, .radius = 0.3, .color = {0, 255, 255}, .specular = 500, .reflective = 0.9});
  scene.spheres.push_back({.center = {0, 1.2, 9}, .radius = 1, .color = {0, 255, 255}, .specular = 500, .reflective = 1.0});
  // scene.spheres.push_back({.center = {0, 18, 9}, .radius = 10, .color = {0, 255, 255}, .specular = 500, .reflective = 1.0});

  scene.ambient_intensity = 0.2f;
  scene.lights.push_back(std::make_unique<PointLight>(Vec3<float>{2,1,0}, 0.6f));
  scene.lights.push_back(std::make_unique<DirectionalLight>(Vec3<float>{1,4,4}, 0.2f));

  // Raytracing algorithm
  for (int x = -(kWidth/2); x < (kWidth/2); x++) {
    for (int y = -(kHeight/2) + 1; y <= (kHeight/2); y++) {
      const Vec3<float> viewport_pos = CanvasToViewport({x, y});
      const Ray ray_out_viewport = viewport_pos - kCameraPos; // D = V - O (from O to V)
      const Color color = TraceRay(scene, kCameraPos, ray_out_viewport, 1.0f, kInf, kRecursionDepth);
      img->SetPixel(CanvasToScreen({x,y}), color);
    }
  }
}

void dbg() {
  Mat<float> m(4,4);
  m(0,0) = 2.3f;
  m(1,1) = 2.0f;
  std::cout << m << std::endl;
  Mat<float> m2 = m;
  std::cout << m2 << std::endl;

  Mat<float> m3(3, 2);
  m3(0,0)=1;
  m3(1,0)=2;
  m3(2,0)=3;
  m3(0,1)=4;
  m3(1,1)=5;
  m3(2,1)=6;
  std::cout << m3 << std::endl;
  Mat<float> m4(2, 3);
  m4(0,0)=7;
  m4(1,0)=8;
  m4(0,1)=9;
  m4(1,1)=10;
  m4(0,2)=11;
  m4(1,2)=12;
  std::cout << m4 << std::endl;
  Mat<float> m5 = m3 * m4;
  std::cout << m5 << std::endl;

  Mat<float> m6(3, 2);
  m6(0,0)=1;
  m6(1,0)=-1;
  m6(2,0)=2;
  m6(0,1)=0;
  m6(1,1)=-3;
  m6(2,1)=1;
  Vec3<float> v = {2,1,0};
  Mat<float> m7 = m6 * v;
  std::cout << m7 << std::endl;
}

int main(void) {
  dbg();
  SDLViewer viewer("Raytracing Renderer", kWidth, kHeight);
  
  Image img(kWidth, kHeight);
  img.SetAll(0);
  try {
    const auto start_time = Clock::now();
    SetImage(&img);
    std::chrono::duration<double> diff = Clock::now() - start_time;
    std::cout << string_format("Rendered %d rays up to %d times in %ldms\n", kWidth * kHeight,
          kRecursionDepth,
          std::chrono::duration_cast<std::chrono::milliseconds>(diff).count());
  } catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }

  viewer.SetImage(img);
  bool quit = false;
  while (!quit) {
    auto events = viewer.Update();
    for (const auto& e : events) {
      if (e.type == SDL_QUIT) {
        quit = true;
        break;
      }
    }
  }

  return 0;
}