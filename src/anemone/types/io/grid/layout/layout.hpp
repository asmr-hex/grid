/**
 * @file   types/io/grid/layout/layout.hpp
 * @brief  IO Grid Layout Base Class Type
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef TYPES_IO_GRID_LAYOUT_H
#define TYPES_IO_GRID_LAYOUT_H

#include <map>
#include <vector>
#include <memory>

#include "anemone/types/io/grid/event.hpp"
#include "anemone/types/io/grid/address.hpp"
#include "anemone/types/io/grid/device/event.hpp"
#include "anemone/types/io/grid/device/coordinates.hpp"
#include "anemone/types/io/grid/layout/names.hpp"
#include "anemone/types/io/grid/layout/section.hpp"


/// @brief Base class for grid layouts.
///
/// @details
/// 
class Layout {
public:
  LayoutName name;
  
  Layout(LayoutName name) : name(name) {};
  virtual ~Layout() = default;

  /// @brief Translates an inbound `grid_device_event_t` to a more specific `grid_event_t`.
  ///
  /// @param event   a `grid_device_event_t` originating from the grid device.
  ///
  /// @ return a high level `grid_event_t`.
  ///
  grid_event_t translate(const grid_device_event_t& event) const;

  /// @brief Translates a high-level `grid_addr_t` to a low-level `grid_coordinates_t`.
  ///
  /// @param address   a high-level `grid_addr_t` address
  ///
  /// @return a low-level `grid_coordinates_t` coordinate
  ///
  grid_coordinates_t translate(const grid_addr_t&) const;

  /// @brief a map of sections by name.
  std::map< GridSectionName, std::shared_ptr<GridSection> > section_by_name;
  
protected:
  void register_section(std::shared_ptr<GridSection>);
  
private:
  std::vector<std::shared_ptr<GridSection> > sections;

  /// @brief returns the grid section corresponding to the provided grid coordinates.
  ///
  /// @param grid_coordinates    `grid_coordinates_t` to fidn the section of.
  ///
  /// @return grid section   ptr to a `GridSection`
  ///
  std::shared_ptr<GridSection> section_of(const grid_coordinates_t&) const;
};

#endif
