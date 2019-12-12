#ifndef IO_GRID_LAYOUT_H
#define IO_GRID_LAYOUT_H

#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/device/coordinates.hpp"
#include "anemone/io/grid/layout/address.hpp"
#include "anemone/io/grid/layout/names.hpp"

// container of grid sections
// maps incoming grid events to sections to generate grid events
//
// what will subscribe to sections of a layout?
// controllers
// what will use the dimension methods of sections of a layout?
// controllers
//
// we should pass already instantiated layouts to the Grid which will instantiate a LayoutContext

class LayoutContext;  // forward-declaration

class Layout {
public:
  LayoutName name;
  
  Layout(LayoutName name) : name(name);
  virtual ~Layout() = default;

  void notify(LayoutContext&, grid_device_event_t&&);
  grid_coordinates_t translate(grid_addr_t&&);

private:
  // TODO implement algorithm for choosing layout section
  // given a set of coordinates! (basically a better implementation
  // of the "relevant" variable within the old handlers)
};


// nest Section class! for namespace treats!
#endif
