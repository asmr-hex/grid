#include "anemone/io/grid/grid.hpp"


Grid::Grid(std::shared_ptr<Config> config,
           std::shared_ptr<GridDevice> device,
           layout_initializer_list layouts)
  : layout(layouts), device(device), device_addr(config->grid_address) {}

void Grid::connect() {
  // TODO ?  connect with address
  subscribe(*device);  // QUESTION: why are we dereferencing here?

  device->connect(device_addr);
}

void Grid::handle(const grid_device_event_t& device_event) {
  layout.notify(device_event);
}
