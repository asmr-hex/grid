#include "anemone/io/grid/grid.hpp"


Grid::Grid(std::shared_ptr<Config> config,
           std::shared_ptr<GridDevice> device,
           layout_initializer_list layouts)
  : layout(layouts), device_addr(config->ports.grid), device(device) {}

void Grid::connect() {
  subscribe(*device);
  device->connect(device_addr);
}

void Grid::handle(const grid_device_event_t& device_event) {
  layout.notify(device_event);
}
