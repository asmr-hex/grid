#include "anemone/io/grid/grid.hpp"


void Grid::connect() {
  subscribe(*device);
}

void Grid::handle(const grid_device_event_t& device_event) {
  layout.notify(device_event);
}
