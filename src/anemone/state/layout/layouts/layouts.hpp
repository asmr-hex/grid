/**
 * @file   state/layout/layouts/layouts.hpp
 * @brief  IO Grid API Class
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef STATE_LAYOUT_LAYOUTS_H
#define STATE_LAYOUT_LAYOUTS_H

#include <memory>

#include "anemone/config.hpp"
#include "anemone/types.hpp"

#include "anemone/state/layout/layouts/sequencer.hpp"


class GridLayouts {
public:
  std::shared_ptr<GridLayout::Sequencer> sequencer;

  GridLayouts(std::shared_ptr<Config>);

  /// @brief A map from `LayoutName` to `Layout` pointers
  std::map< LayoutName, std::shared_ptr<Layout> > layout_by_name;
};

#endif
