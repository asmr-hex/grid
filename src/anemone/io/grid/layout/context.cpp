#include "anemone/io/grid/layout/context.hpp"


LayoutContext::LayoutContext(layout_initializer_list layouts) {
  if (layouts.size() == 0)
    throw std::length_error("no layouts provided");

  for (auto layout : layouts)
    layout_by_name[layout->name] = layout;

  use_layout((*layouts.begin())->name);
}


void LayoutContext::notify(const grid_device_event_t& grid_device_event) {
  current_layout->notify(*this, grid_device_event);
}


grid_coordinates_t LayoutContext::translate(const grid_addr_t& grid_addr) {
  return current_layout->translate(grid_addr);
}


void LayoutContext::use_layout(LayoutName name) {
  current_layout = layout_by_name[name];
}
