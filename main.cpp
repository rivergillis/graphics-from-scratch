#include "common.h"
#include "sdl_viewer.h"
#include "image.h"
#include "vec.h"
#include "sphere.h"
#include "raytrace.h"

void SetImage(Image* img) {
  std::vector<Sphere> scene;
  scene.push_back({.center = {0, -1, 3}, .radius = 1, .color = {255, 0, 0}});
  scene.push_back({.center = {2, 0, 4}, .radius = 1, .color = {0, 0, 255}});
  scene.push_back({.center = {-2, 0, 4}, .radius = 1, .color = {0, 255, 0}});

  // Raytracing algorithm
  for (int x = -(kWidth/2); x < (kWidth/2); x++) {
    for (int y = -(kHeight/2) + 1; y <= (kHeight/2); y++) {
      const Vec3<float> viewport_pos = CanvasToViewport({x, y});
      // Why does this only work for O - V?????
      // const Ray ray_out_viewport = kCameraPos - viewport_pos; // D = V - O (from O to V)
      const Ray ray_out_viewport = viewport_pos - kCameraPos; // D = V - O (from O to V)
      const Color color = TraceRay(scene, kCameraPos, ray_out_viewport, 1.0f, kInf);
      const Vec2<int> screen_pos = CanvasToScreen({x, y});
      std::cout << string_format("x %d y %d viewport_pos %s ray_out_viewport %s color %s Screen xy %s\n", x, y, ToString(viewport_pos).c_str(), ToString(ray_out_viewport).c_str(), ToString(color).c_str(), ToString(screen_pos).c_str());
      img->SetPixel(screen_pos, color);
    }
  }
}

int main(void) {
  SDLViewer viewer("Renderer", kWidth, kHeight);
  
  Image img(kWidth, kHeight);
  img.SetAll(0);
  try {
    SetImage(&img);
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