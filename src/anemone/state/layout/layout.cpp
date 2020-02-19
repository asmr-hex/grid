#include "anemone/state/layout/layout.hpp"

#include <spdlog/spdlog.h>


grid_event_t Layout::translate(const grid_device_event_t& event) const {
  GridEvent type;
  std::shared_ptr<GridSection> section;
  
  try {
    section = section_of(device_event); 
  } catch (...) {
    spdlog::warn("Grid button ({}, {}) belongs to so section!", device_event.x, device_event.y);
    return;
  }

  switch (device_event.type) {
  case GridDeviceEvent::ButtonDown:
    type = GridEvent::Pressed;
    break;
  case GridDeviceEvent::ButtonUp:
    type = GridEvent::Unpressed;
    break;
  default:
    throw "event unsupported!";
  }
  
  return {{
           .layout  = name,
           .section = section->name,
           .index   = section->index_of(device_event),
           },
          .type     = type,
  };
}

grid_coordinates_t Layout::translate(const grid_addr_t& addr) const {
  auto section = section_by_name.at(addr.section);

  return section->coordinates_of(addr.index);
}

void Layout::register_section(std::shared_ptr<GridSection> section) {
  sections.push_back(section);
  section_by_name[section->name] = section;
}

std::shared_ptr<GridSection> Layout::section_of(const grid_coordinates_t& coordinates) const {
  for (auto section : sections) {
    if ( section->contains(coordinates) )
      return section;
  }

  // TODO (coco|1.18.2020) make this an actual exception type.
  throw "no section found!";
}
