/**
 * @file   io/grid/layout/context.hpp
 * @brief  IO Grid Layout Context Class
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_LAYOUT_CONTEXT_H
#define IO_GRID_LAYOUT_CONTEXT_H

#include <map>
#include <memory>
#include <initializer_list>

#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/device/coordinates.hpp"
#include "anemone/io/grid/layout/layout.hpp"
#include "anemone/io/grid/address.hpp"
#include "anemone/io/grid/layout/names.hpp"


/// @brief type alias for an initializer list of layout shared pointers.
typedef std::initializer_list< std::shared_ptr<Layout> > layout_initializer_list;


/// @brief Layout context for accessing current and inactive layouts.
///
/// @details
/// The `LayoutContext` class implements the state design pattern for storing and
/// internally switching the current layout.
///
class LayoutContext {
public:
  /// @brief Constructs a `LayoutContext` and sets the current layout.
  ///
  /// @param layouts   an initializer list of layouts to use.
  ///
  LayoutContext(layout_initializer_list layouts);

  /// @brief Notifies the current layout of an inbound `grid_device_event_t`.
  ///
  /// @details
  /// This method delegates translation and re-broadcast of the inbound event to the current
  /// layout, which in tern delegates translation and re-broadcast to the relevant layout section.
  ///
  /// @param event   a `grid_device_event_t` originating from the grid device.
  ///
  void notify(const grid_device_event_t& event);

  /// @brief Translates a high-level `grid_addr_t` to a low-level `grid_coordinates_t`.
  ///
  /// @details
  /// This method delegates translation to the current layout, which in tern delegates translation
  /// to the relevant layout section.
  ///
  /// @param address   a high-level `grid_addr_t` address
  ///
  /// @return a low-level `grid_coordinates_t` coordinate
  ///
  grid_coordinates_t translate(const grid_addr_t&);

  /// @brief Gets the name of the current layout
  ///
  /// @return a `LayoutName`
  ///
  LayoutName name() { return current_layout->name; };
  
private:
  /// @brief A pointer to the current layout
  std::shared_ptr<Layout> current_layout;

  /// @brief A map from `LayoutName` to `Layout` pointers
  std::map< LayoutName, std::shared_ptr<Layout> > layout_by_name;

  /// @brief Switches the current layout to a new layout
  ///
  /// @param new_layout   the new layout to use
  ///
  void use_layout(LayoutName new_layout);

  friend class Layout;
};


#endif
