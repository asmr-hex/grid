/**
 * @file   state/layout/context.hpp
 * @brief  Grid Layout Context Class
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef STATE_GRID_LAYOUT_CONTEXT_H
#define STATE_GRID_LAYOUT_CONTEXT_H

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
class LayoutContext : public rxcpp::subjects::behavior< std::shared_ptr<Layout> > {
public:
  /// @brief Constructs a `LayoutContext` and sets the current layout.
  ///
  /// @param layouts   an initializer list of layouts to use.
  ///
  LayoutContext(layout_initializer_list layouts);

  /// @brief Switches the current layout to a new layout
  ///
  /// @param new_layout   the new layout to use
  ///
  void use_layout(LayoutName new_layout);

private:
  /// @brief A pointer to the current layout
  std::shared_ptr<Layout> current_layout;

  /// @brief A map from `LayoutName` to `Layout` pointers
  std::map< LayoutName, std::shared_ptr<Layout> > layout_by_name;
};


#endif
