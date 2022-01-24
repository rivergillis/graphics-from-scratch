#include "common.h"
#include "sdl_viewer.h"
#include "image.h"
#include "vec.h"
#include "sphere.h"
#include "raytrace.h"
#include "scene.h"

void SetImage(Image* img) {
  Scene scene;
  scene.spheres.push_back({.center = {0, -1, 3}, .radius = 1, .color = {255, 0, 0}});
  scene.spheres.push_back({.center = {2, 0, 4}, .radius = 1, .color = {0, 0, 255}});
  scene.spheres.push_back({.center = {-2, 0, 4}, .radius = 1, .color = {0, 255, 0}});
  scene.spheres.push_back({.center = {0, -5001, 0}, .radius = 5000, .color = {255, 255, 0}});

  scene.ambient_intensity = 0.2f;
  scene.point_lights.push_back({.intensity = 0.6f, .position = {2, 1, 0}});
  scene.directional_lights.push_back({.intensity = 0.2f, .direction = {1, 4, 4}});

  // Raytracing algorithm
  for (int x = -(kWidth/2); x < (kWidth/2); x++) {
    for (int y = -(kHeight/2) + 1; y <= (kHeight/2); y++) {
      const Vec3<float> viewport_pos = CanvasToViewport({x, y});
      const Ray ray_out_viewport = viewport_pos - kCameraPos; // D = V - O (from O to V)
      const Color color = TraceRay(scene, kCameraPos, ray_out_viewport, 1.0f, kInf);
      img->SetPixel(CanvasToScreen({x,y}), color);
    }
  }
}

int main(void) {
  SDLViewer viewer("Renderer", kWidth, kHeight);
  
  Image img(kWidth, kHeight);
  img.SetAll(0);
  try {
    const auto start_time = Clock::now();
    SetImage(&img);
    std::chrono::duration<double> diff = Clock::now() - start_time;
    std::cout << string_format("Rendered %d rays in %ldms\n", kWidth * kHeight,
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