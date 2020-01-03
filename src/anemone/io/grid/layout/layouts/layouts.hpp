#ifndef IO_GRID_LAYOUT_LAYOUTS_H
#define IO_GRID_LAYOUT_LAYOUTS_H

#include <memory>

#include "anemone/config/config.hpp"
#include "anemone/io/grid/layout/layouts/sequencer.hpp"


class GridLayouts {
public:
  std::shared_ptr<GridLayout::Sequencer> sequencer;

  GridLayouts(std::shared_ptr<Config>);
};

#endif
