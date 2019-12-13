#include "anemone/io/grid/layout/layout.hpp"


void Layout::notify(LayoutContext& ctx, const grid_device_event_t& event) {
  GridSection& section = section_of(event);

  // TODO broadcast translated grid_event!
  // TODO we need to define the grid evennt data type!
}


grid_coordinates_t Layout::translate(const grid_addr_t& addr) {
  GridSection& section = section_by_name[addr.section];

  return section.coordinates_of(addr.index);
}


GridSection& Layout::section_of(const grid_coordinates_t& c) {
  for (GridSection& section : sections) {
    if ( section.contains(coordinates) )
      return section;
  }

  throw "no section found!";
}
