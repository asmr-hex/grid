/**
 * @file   io/grid/layout/layout.hpp
 * @brief  IO Grid Layout Base Class
 * @author coco
 * @date   2020-01-15
 *************************************************/


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


// forward-declaration
class LayoutContext;


/// @brief Base class for grid layouts.
///
/// @details
/// 
class Layout {
public:
  LayoutName name;
  
  Layout(LayoutName name) : name(name) {};
  virtual ~Layout() = default;

  // accept LayoutContext in order to change layout from within handler.
  void notify(LayoutContext&, const grid_device_event_t&) const;
  grid_coordinates_t translate(const grid_addr_t&) const;

protected:
  void register_section(std::shared_ptr<GridSection>);
  virtual std::optional<LayoutName> change_layout(const grid_event_t&) const;
  
private:
  std::vector<std::shared_ptr<GridSection> > sections;
  std::map< GridSectionName, std::shared_ptr<GridSection> > section_by_name;

  // we introduce this wrapper because LayoutContext declares Layout as a friend
  // class, though friend-ness isn't derivable. thus any deriving classes can't
  // access LayoutContext's protected 'use_layout' method.
  void change_layout_wrapper(LayoutContext&, const grid_event_t&) const;

  grid_event_t translate(std::shared_ptr<GridSection>, const grid_device_event_t&) const;
  std::shared_ptr<GridSection> section_of(const grid_coordinates_t&) const;
};

#endif
