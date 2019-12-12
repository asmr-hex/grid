#include "anemone/io/grid/layout/context.hpp"


LayoutContext::LayoutContext(layout_initializer_list layouts) {
  if (layouts.size() == 0)
    throw std::length_error;

  for (auto layout : layouts)
    layout_by_name[layout->name()] = layout;

  use_layout(layouts[0]->name());
}


void LayoutContext::notify(grid_device_event_t&& grid_device_event) {
  current_layout->notify(this, std::forward(grid_device_event));
}


grid_coordinates_t LayoutContext::translate(grid_addr_t&& grid_addr) {
  return current_layout->transform(std::forward(grid_addr));
}


void LayoutContext::use_layout(LayoutName name) {
  current_layout = layout_by_name[name];
}
