#include <spdlog/spdlog.h>

#include "anemone/io/grid/animation/animator.hpp"


Animator::Animator(std::shared_ptr<GridDevice> grid_device, std::shared_ptr<Layout> current_layout)
  : grid_device(grid_device),
    current_layout(current_layout)
{}

void Animator::run() {
  // begin animation loop
  std::thread t([this] () {
                  auto t = std::chrono::milliseconds(0);
                  
                  while (true) {
                    auto t1 = std::chrono::high_resolution_clock::now();

                    render(t);
                    t += period;
                    
                    auto t2 = std::chrono::high_resolution_clock::now();
                    auto wait = period - std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t2);

                    std::this_thread::sleep_for(wait);
                  }
                });

  t.detach();
}

void Animator::render(std::chrono::milliseconds t) {
  std::lock_guard<std::mutex> guard(lock);

  for (auto itr : pixels) {
    auto coordinates = itr.first;
    auto animation   = itr.second;

    auto intensity = animation->compute(t);
    
    grid_device->set(coordinates, intensity);
  }
}

void Animator::add(std::shared_ptr<Animation> animation, grid_addr_t grid_addr) {
  std::lock_guard<std::mutex> guard(lock);

  // translate grid_addr to grid_coordinates.
  auto coordinates = current_layout->translate(grid_addr);

  pixels[coordinates] = animation;
}

void Animator::add(std::unordered_map<grid_addr_t, std::shared_ptr<Animation>, grid_addr_hasher> grid_addr_to_animation) {
  for (auto itr : grid_addr_to_animation) {
    add(itr.second, itr.first);
  }
}

void Animator::remove(grid_addr_t grid_addr) {
  std::lock_guard<std::mutex> guard(lock);
  
  // translate grid_addr to grid_coordinates.
  auto coordinates = current_layout->translate(grid_addr);
  
  pixels.erase(coordinates);
}

void Animator::remove(grid_addr_t grid_addr, unsigned int intensity) {
  std::lock_guard<std::mutex> guard(lock);
  
  // translate grid_addr to grid_coordinates.
  auto coordinates = current_layout->translate(grid_addr);

  pixels.erase(coordinates);

  grid_device->set(coordinates, intensity);
}

void Animator::remove(std::vector<grid_addr_t> grid_addrs) {
  for (auto grid_addr : grid_addrs) {
    remove(grid_addr);
  }
}

void Animator::remove(std::vector<grid_addr_t> grid_addrs, unsigned int intensity) {
  for (auto grid_addr : grid_addrs) {
    remove(grid_addr, intensity);
  }  
}
