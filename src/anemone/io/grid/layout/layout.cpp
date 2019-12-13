#include "anemone/io/grid/layout/layout.hpp"


void Layout::notify(LayoutContext& ctx, const grid_device_event_t& device_event) {
  GridSection& section = section_of(device_event);
  grid_event_t grid_event = translate(section, device_event);

  // TODO if the device event is triggering a new layout, use ctx.use_layout...is this a code smell?
  
  section.broadcast(grid_event);
}


grid_coordinates_t Layout::translate(const grid_addr_t& addr) {
  auto section = section_by_name[addr.section];

  return section->coordinates_of(addr.index);
}

grid_event_t Layout::translate(GridSection& section, const grid_device_event_t& device_event) {
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
           .section = section.name,
           .index   = section.index_of(device_event),
           },
          .type     = type,
  };
}

GridSection& Layout::section_of(const grid_coordinates_t& coordinates) {
  for (auto section : sections) {
    if ( section->contains(coordinates) )
      return *section;
  }

  throw "no section found!";
}
