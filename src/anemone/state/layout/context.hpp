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

#include "anemone/rx.hpp"
#include "anemone/types.hpp"
#include "anemone/state/layout/layout.hpp"
#include "anemone/state/layout/layouts/layouts.hpp"


/// @brief Layout context for accessing current and inactive layouts.
///
/// @details
/// The `LayoutContext` class implements the state design pattern for storing and
/// internally switching the current layout.
///
class LayoutContext : public rx::behavior< std::shared_ptr<Layout> > {
public:
  /// @brief Constructs a `LayoutContext` and sets the current layout.
  ///
  LayoutContext(std::shared_ptr<GridLayouts> layouts);

  /// @brief Switches the current layout to a new layout
  ///
  /// @param new_layout   the new layout to use
  ///
  void use_layout(LayoutName new_layout);

  /// @brief Get a collection of grid layouts
  ///
  /// @return ptr to `GridLayouts`
  ///
  std::shared_ptr<GridLayouts> get_layouts();
  
private:
  /// @brief A pointer to the current layout
  std::shared_ptr<Layout> current_layout;

  /// @brief A pointer to a collection of all the grid layouts
  std::shared_ptr<GridLayouts> layouts;
};


#endif
