#ifndef IO_GRID_LAYOUT_H
#define IO_GRID_LAYOUT_H

#include <map>
#include <vector>
#include <memory>
#include <optional>

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
  void notify(LayoutContext&, const grid_device_event_t&) const;
  grid_coordinates_t translate(const grid_addr_t&) const;

protected:
  void register_section(GridSection&);
  virtual std::optional<LayoutName> change_layout(const grid_event_t&) const;
  
private:
  std::vector<GridSection *> sections;
  std::map< GridSectionName, GridSection *> section_by_name;

  // we introduce this wrapper because LayoutContext declares Layout as a friend
  // class, though friend-ness isn't derivable. thus any deriving classes can't
  // access LayoutContext's protected 'use_layout' method.
  void change_layout_wrapper(LayoutContext&, const grid_event_t&) const;

  grid_event_t translate(const GridSection&, const grid_device_event_t&) const;
  GridSection& section_of(const grid_coordinates_t&) const;
};

#endif
