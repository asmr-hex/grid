/**
 * @file   state/layouts/layouts.hpp
 * @brief  IO Grid Layout Container Class
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef STATE_LAYOUTS_H
#define STATE_LAYOUTS_H


#include <memory>

#include "anemone/config.hpp"
#include "anemone/types.hpp"

#include "anemone/state/layouts/sequencer.hpp"


class GridLayouts {
public:
  /// @brief the Sequencer Layout.
  std::shared_ptr<GridLayout::Sequencer> sequencer;

  /// @brief A map from `LayoutName` to `Layout` pointers
  std::map< LayoutName, std::shared_ptr<Layout> > layout_by_name;

  /// @brief the current layout
  rx::behavior< std::shared_ptr<Layout> > current_layout;
  
  /// @brief constructs all layouts from the provided config.
  GridLayouts(std::shared_ptr<Config>);
};

#endif
