#include "anemone/io/grid/layout/layout.hpp"
#include "anemone/io/grid/layout/context.hpp"


void Layout::notify(LayoutContext& ctx, const grid_device_event_t& device_event) const {
  GridSection& section = section_of(device_event);
  grid_event_t grid_event = translate(section, device_event);

  section.broadcast(grid_event);

  // optionally switch layout.
  change_layout_wrapper(ctx, grid_event);
}


grid_coordinates_t Layout::translate(const grid_addr_t& addr) const {
  auto section = section_by_name.at(addr.section);

  return section->coordinates_of(addr.index);
}

grid_event_t Layout::translate(const GridSection& section, const grid_device_event_t& device_event) const {
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

void Layout::register_section(GridSection& section) {
  sections.push_back(&section);
  section_by_name[section.name] = &section;
}

void Layout::change_layout_wrapper(LayoutContext& ctx, const grid_event_t& event) const {
  auto new_layout = change_layout(event);
  if ( new_layout.has_value() )
    ctx.use_layout(new_layout.value());
};

std::optional<LayoutName> Layout::change_layout(const grid_event_t& event) const { return std::nullopt; };

GridSection& Layout::section_of(const grid_coordinates_t& coordinates) const {
  for (auto section : sections) {
    if ( section->contains(coordinates) )
      return *section;
  }

  throw "no section found!";
}
