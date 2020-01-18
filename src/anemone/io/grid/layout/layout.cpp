#include "anemone/io/grid/layout/layout.hpp"
#include "anemone/io/grid/layout/context.hpp"

#include <spdlog/spdlog.h>


void Layout::notify(LayoutContext& ctx, const grid_device_event_t& device_event) const {
  std::shared_ptr<GridSection> section;
  try {
    section = section_of(device_event); 
  } catch (...) {
    spdlog::warn("Grid button ({}, {}) belongs to so section!", device_event.x, device_event.y);
    return;
  }
  
  grid_event_t grid_event = translate(section, device_event);

  section->broadcast(grid_event);

  // optionally switch layout.
  change_layout_wrapper(ctx, grid_event);
}


grid_coordinates_t Layout::translate(const grid_addr_t& addr) const {
  auto section = section_by_name.at(addr.section);

  return section->coordinates_of(addr.index);
}

grid_event_t Layout::translate(std::shared_ptr<GridSection> section, const grid_device_event_t& device_event) const {
  GridEvent type;

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

void Layout::register_section(std::shared_ptr<GridSection> section) {
  sections.push_back(section);
  section_by_name[section->name] = section;
}

void Layout::change_layout_wrapper(LayoutContext& ctx, const grid_event_t& event) const {
  auto new_layout = change_layout(event);
  if ( new_layout.has_value() )
    ctx.use_layout(new_layout.value());
};

std::optional<LayoutName> Layout::change_layout(const grid_event_t& event) const { return std::nullopt; };

std::shared_ptr<GridSection> Layout::section_of(const grid_coordinates_t& coordinates) const {
  for (auto section : sections) {
    if ( section->contains(coordinates) )
      return section;
  }

  // TODO (coco|1.18.2020) make this an actual exception type.
  throw "no section found!";
}
