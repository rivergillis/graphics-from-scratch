#include "common.h"
#include "sdl_viewer.h"
#include "image.h"
#include "vec.h"
#include "sphere.h"
#include "raytrace.h"
#include "scene.h"
#include "mat.h"
#include "ping_pong.h"

// Values for animating the scene
std::vector<PingPong<float>> a;
void InitAnim() {
  // Camera rotation
  a.emplace_back(0, 180, 0.5f);
  // One property per sphere, some sphere missing
  a.emplace_back(-1, 1, 0.1);
  a.emplace_back(0.5, 1.2, 0.1);
  a.emplace_back(0.3, 1.3, 0.01);
  a.emplace_back(-0.5, 0.6, 0.05);
  a.emplace_back(-1, 2.0, 0.08);
  a.emplace_back(-0.5, 1.5, 0.09);
  a.emplace_back(4, 7, 0.1);
}

void InitScene(Scene* scene) {
  scene->spheres.push_back({.center = {0, -1, 3}, .radius = 1, .color = {255, 0, 0}, .specular = 500, .reflective = 0.2});
  scene->spheres.push_back({.center = {2, 0, 4}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {-2, 0, 4}, .radius = 1, .color = {0, 255, 0}, .specular = 10, .reflective = 0.4});
  scene->spheres.push_back({.center = {0, -5001, 0}, .radius = 5000, .color = {255, 255, 0}, .specular = 1000, .reflective = 0.5});
  scene->spheres.push_back({.center = {0, 1.2, 9}, .radius = 1, .color = {0, 255, 255}, .specular = 500, .reflective = 1.0});

  scene->spheres.push_back({.center = {7, 1, 4}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {5, 5, 3}, .radius = 3, .color = {224, 187, 228}, .specular = 500, .reflective = 0.01});
  scene->spheres.push_back({.center = {5, 0.5, 3}, .radius = 1, .color = {149, 125, 173}, .specular = 500, .reflective = 0.01});
  scene->spheres.push_back({.center = {7, 0.3, 2}, .radius = 0.7, .color = {210, 145, 188}, .specular = 500, .reflective = 0.01});
  scene->spheres.push_back({.center = {7, 0.7, 0.4}, .radius = 0.7, .color = {254, 200, 216}, .specular = 500, .reflective = 0.01});
  scene->spheres.push_back({.center = {3, -0.5, 0.1}, .radius = 0.2, .color = {0, 0, 255}, .specular = 500, .reflective = 0.01});
  scene->spheres.push_back({.center = {20, 3, 0}, .radius = 4, .color = {0, 0, 255}, .specular = 500, .reflective = 1.0});
  scene->spheres.push_back({.center = {5, 0, -1}, .radius = 0.7, .color = {255, 223, 211}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {6, 0, -3}, .radius = 0.7, .color = {255, 255, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {4, 1, -5}, .radius = 0.7, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});

  scene->spheres.push_back({.center = {5, 2, -4}, .radius = 1, .color = {232, 244, 234}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {6, 0, -6}, .radius = 1, .color = {255, 187, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {4, 0, -7}, .radius = 1, .color = {0, 100, 255}, .specular = 500, .reflective = 0.3});

  scene->spheres.push_back({.center = {0, 5, -24}, .radius = 5, .color = {224, 240, 227}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {2, 1, -5}, .radius = 1, .color = {210, 231, 214}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {2, 0, -7}, .radius = 1, .color = {0, 255, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {-1, 0, -5}, .radius = 1, .color = {0, 255, 0}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {-2, 0, -2}, .radius = 0.5, .color = {184, 216,190}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {-5, 0, -2}, .radius = 0.5, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});

  scene->ambient_intensity = 0.2f;
  scene->lights.push_back(std::make_unique<PointLight>(Vec3<float>{2,1,0}, 0.6f));
  scene->lights.push_back(std::make_unique<DirectionalLight>(Vec3<float>{1,4,4}, 0.2f));

  scene->recursion_depth = 5;
  scene->camera_position = {0, 0, 0};
}

void UpdateScene(Scene* scene, uint64_t i) {
  scene->camera_rotation = Mat<float>::RotateY(a[0].Next());
  scene->spheres[0].center.x = a[1].Next();
  scene->spheres[2].center.y = a[2].Next();
  scene->spheres[8].center.y = a[3].Next();
  scene->spheres[9].center.y = a[4].Next();
  scene->spheres[10].center.y = a[5].Next();
  scene->spheres[13].center.y = a[6].Next();
  scene->spheres[18].center.y = a[7].Next();
}

void Render(const Scene& scene, Image* img) {
  // Raytracing algorithm
  for (int x = -(kWidth/2); x < (kWidth/2); x++) {
    for (int y = -(kHeight/2) + 1; y <= (kHeight/2); y++) {
      const Vec3<float> viewport_pos = (scene.camera_rotation * CanvasToViewport({x, y})).ToVec3();
      const Ray ray_out_viewport = viewport_pos - scene.camera_position;
      const Color color = TraceRay(scene, scene.camera_position, ray_out_viewport, 1.0f, kInf, scene.recursion_depth);
      img->SetPixel(CanvasToScreen({x,y}), color);
    }
  }
}

int main(void) {
  InitAnim();
  SDLViewer viewer("Raytracing Renderer", kWidth, kHeight);
  
  Image img(kWidth, kHeight);
  img.SetAll(0);
  viewer.SetImage(img);

  Scene scene;
  InitScene(&scene);

  bool quit = false;
  uint64_t i = 0;
  while (!quit) {
    try {
      UpdateScene(&scene, ++i);
      const auto start_time = Clock::now();
      Render(scene, &img);
      std::chrono::duration<double> diff = Clock::now() - start_time;
      std::cout << string_format("Rendered %d rays in %ldms\n", kWidth * kHeight,
            std::chrono::duration_cast<std::chrono::milliseconds>(diff).count());
    } catch (const std::exception& e) {
      std::cerr << "ERROR: " << e.what() << std::endl;
      return 1;
    }

    viewer.SetImage(img);
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