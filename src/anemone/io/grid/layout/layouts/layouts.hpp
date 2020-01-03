#ifndef IO_GRID_LAYOUT_LAYOUTS_H
#define IO_GRID_LAYOUT_LAYOUTS_H

#include <memory>

#include "anemone/io/grid/layout/layouts/sequencer.hpp"


struct GridLayouts {
  std::shared_ptr<GridLayout::Sequencer> sequencer;
};

#endif
