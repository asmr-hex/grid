#include "anemone/types/io/grid/layout/layout.hpp"

#include <spdlog/spdlog.h>


Layout::Layout(LayoutName name)
  : name(name)
{}

grid_event_t Layout::translate(const grid_device_event_t& device_event) const {
  GridEvent type;
  std::shared_ptr<GridSection> section;
  LayoutName layout_name;
  grid_coordinates_t coordinates;
  
  try {
    // get the layout, section, and coordinates of this device event.
    auto t = layout_section_and_coords_of(device_event);
    layout_name = std::get<0>(t);
    section     = std::get<1>(t);
    coordinates = std::get<2>(t);
  } catch (...) {
    spdlog::warn("Grid button ({}, {}) belongs to no section!", device_event.x, device_event.y);
    return {};
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
           .layout  = layout_name,
           .section = section->name,
           .index   = section->index_of(coordinates),
           },
          .type     = type,
  };
}

grid_coordinates_t Layout::translate(const grid_addr_t& addr) const {
  auto section = section_by_name.at(addr.section);

  auto coordinates = section->coordinates_of(addr.index);
  
  // if this layout has a parent layout
  if (superlayout.layout != nullptr) {
    // add the section offsets to the coordinates
    coordinates.x += section->region().min.x;
    coordinates.y += section->region().min.y;

    // re-construct the corresponding grid address of the parent layout's section
    // and call translate on it. this will cause the translation to bubble up until
    // there is no parent layout in case there are deeply nested layouts.
    coordinates = superlayout.layout->translate
      ({ .layout  = superlayout.layout->name,
         .section = superlayout.section,
         .index   = superlayout.layout->section_by_name[superlayout.section]->index_of(coordinates),
      });
  }

  return coordinates;
}

void Layout::connect(std::shared_ptr<State> state) {
  // register sections
  register_sections();

  // iterate over all registered sublayouts and invoke this method on them.
  for (auto sublayout : sublayouts) {
    sublayout->connect(state);
  }
  
  // iterate over all handlers and call them
  for (auto fn : sublayout_update_handlers) {
    fn(state);
  }
}

void Layout::register_section(std::shared_ptr<GridSection> section) {
  sections.push_back(section);
  section_by_name[section->name] = section;
}

void Layout::register_sublayout(GridSectionName section_name, std::shared_ptr<Layout> child_layout) {
  // add to the vector of registered sublayouts
  sublayouts.push_back(child_layout);

  // register this current layout as the child's superlayout.
  child_layout->register_superlayout(section_name, shared_from_this());
}

void Layout::add_sublayout_update_handler(std::function<void(std::shared_ptr<State>)> fn) {
  sublayout_update_handlers.push_back(fn);
}

void Layout::update_sublayout_for(GridSectionName section_name, std::shared_ptr<Layout> sublayout) {
  current_sublayout_by_section[section_name] = sublayout;
}

std::tuple<LayoutName, std::shared_ptr<GridSection>, grid_coordinates_t>
Layout::layout_section_and_coords_of(const grid_coordinates_t& coordinates) const {
  for (auto section : sections) {
    // have we found the relevant section?
    if ( section->contains(coordinates) ) {
      try {
        // does this section have a sublayout?
        auto sublayout = current_sublayout_by_section.at(section->name);

        // cool, there is a sublayout.

        // transform the coordinates to be relative to the sublayout
        auto transformed_coordinates = coordinates;
        transformed_coordinates.x -= section->region().min.x;
        transformed_coordinates.y -= section->region().min.y;

        // call this same method on the sublayout and return results.
        return sublayout->layout_section_and_coords_of(transformed_coordinates);
        
      } catch (std::out_of_range & error) {
        // there was no sublayout for this section, just return this layout and section
        return std::make_tuple(name, section, coordinates);
      }
    }
  }

  // TODO (coco|1.18.2020) make this an actual exception type.
  throw "no section found!";
}

void Layout::register_superlayout(GridSectionName section_name, std::shared_ptr<Layout> parent_layout) {
  // set the parent layout for this layout
  superlayout = { .section = section_name, .layout = parent_layout };
}
