#include "anemone/io/grid/grid.hpp"

#include <spdlog/spdlog.h>


Grid::Grid(std::shared_ptr<Config> config,
           std::shared_ptr<GridDevice> device,
           layout_initializer_list layouts)
  : layout(layouts), device_addr(config->ports.grid), device(device) {}

void Grid::connect() {
  subscribe(*device);
  device->connect(device_addr);
  device->listen();
}

void Grid::turn_off(const grid_addr_t& addr) {
  // send translated address to device
  device->turn_off(layout.translate(addr));
}

void Grid::turn_on(const grid_addr_t& addr) {
  // send translated address to device
  device->turn_on(layout.translate(addr));
}

void Grid::set(const grid_addr_t& addr, unsigned int intensity) {
  // send translated address to device
  device->set(layout.translate(addr), intensity);
}

void Grid::handle(const grid_device_event_t& device_event) {
  layout.notify(device_event);
}
