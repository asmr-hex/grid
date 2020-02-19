#include <spdlog/spdlog.h>

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
                 spdlog::info("SWITCHING TO LAYOUT {}", l->name);
                 current_layout = l;
               });
}

rx::observable<grid_event_t> Grid::connect() {
  device->connect(device_addr);

  return device->listen()
    | rx::map([this] (grid_device_event_t e) {
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
