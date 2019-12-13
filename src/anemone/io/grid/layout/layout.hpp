#ifndef IO_GRID_LAYOUT_H
#define IO_GRID_LAYOUT_H

#include <map>
#include <vector>
#include <memory>

#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/device/coordinates.hpp"
#include "anemone/io/grid/address.hpp"
#include "anemone/io/grid/event.hpp"
#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/layout/section.hpp"


class LayoutContext;  // forward-declaration

class Layout {
public:
  LayoutName name;
  
  Layout(LayoutName name) : name(name) {};
  virtual ~Layout() = default;

  // accept LayoutContext in order to change layout from within handler.
  void notify(LayoutContext&, const grid_device_event_t&);
  grid_coordinates_t translate(const grid_addr_t&);

private:
  std::vector< std::shared_ptr<GridSection> > sections;
  std::map< GridSectionName, std::shared_ptr<GridSection> > section_by_name;

  grid_event_t translate(GridSection&, const grid_device_event_t&);
  
  GridSection& section_of(const grid_coordinates_t&);
};


#endif
