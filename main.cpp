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