#include "common.h"
#include "sdl_viewer.h"
#include "image.h"
#include "vec.h"
#include "sphere.h"
#include "raytrace.h"
#include "scene.h"
#include "mat.h"

void InitScene(Scene* scene) {
  scene->spheres.push_back({.center = {0, -1, 3}, .radius = 1, .color = {255, 0, 0}, .specular = 500, .reflective = 0.2});
  scene->spheres.push_back({.center = {2, 0, 4}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {-2, 0, 4}, .radius = 1, .color = {0, 255, 0}, .specular = 10, .reflective = 0.4});
  scene->spheres.push_back({.center = {0, -5001, 0}, .radius = 5000, .color = {255, 255, 0}, .specular = 1000, .reflective = 0.5});
  scene->spheres.push_back({.center = {0, 1.2, 9}, .radius = 1, .color = {0, 255, 255}, .specular = 500, .reflective = 1.0});

  scene->spheres.push_back({.center = {7, 1, 4}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {5, 5, 3}, .radius = 3, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {5, 0.5, 3}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {7, 0.3, 2}, .radius = 0.7, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {7, 0.7, 0.4}, .radius = 0.7, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {3, -0.5, 0.1}, .radius = 0.2, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {20, 3, 0}, .radius = 4, .color = {0, 0, 255}, .specular = 500, .reflective = 1.0});
  scene->spheres.push_back({.center = {5, 0, -1}, .radius = 0.7, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {6, 0, -3}, .radius = 0.7, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {4, 1, -5}, .radius = 0.7, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});

  scene->spheres.push_back({.center = {5, 2, -4}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {6, 0, -6}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {4, 0, -7}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});

  scene->spheres.push_back({.center = {0, 5, -24}, .radius = 5, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {2, 1, -5}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {2, 0, -7}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {-1, 0, -5}, .radius = 1, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {-2, 0, -2}, .radius = 0.5, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});
  scene->spheres.push_back({.center = {-5, 0, -2}, .radius = 0.5, .color = {0, 0, 255}, .specular = 500, .reflective = 0.3});

  scene->ambient_intensity = 0.2f;
  scene->lights.push_back(std::make_unique<PointLight>(Vec3<float>{2,1,0}, 0.6f));
  scene->lights.push_back(std::make_unique<DirectionalLight>(Vec3<float>{1,4,4}, 0.2f));

  scene->recursion_depth = 5;
  scene->camera_position = {0, 0, 0};
}

void UpdateScene(Scene* scene, uint64_t i) {
  // scene->camera_rotation = Mat<float>::RotateY(i / 2.0f);
  scene->camera_rotation = Mat<float>::RotateY(180);
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