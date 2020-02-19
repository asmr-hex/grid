#include "anemone/io/grid/grid.hpp"


Grid::Grid(std::shared_ptr<Config> config,
           std::shared_ptr<GridDevice> device,
           std::shared_ptr<LayoutContext> layout)
  : device_addr(config->ports.grid),
    device(device)
{
  // subscribe to current layout
  layout->get_observable()
    .subscribe([this] (std::shared_ptr<Layout> l) {
                 current_layout = l;
               });
}

void Grid::connect() {
  device->connect(device_addr);

  return device->listen()
    | rx::map([current_layout] (grid_device_event_t e) {
                return current_layout->translate(e);
              });
}

void Grid::turn_off(const grid_addr_t& addr) {
  // send translated address to device
  device->turn_off(current_layout->translate(addr));
}

void Grid::turn_on(const grid_addr_t& addr) {
  // send translated address to device
  device->turn_on(current_layout->translate(addr));
}

void Grid::set(const grid_addr_t& addr, unsigned int intensity) {
  // send translated address to device
  device->set(current_layout->translate(addr), intensity);
}
