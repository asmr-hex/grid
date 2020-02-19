#include "anemone/state/layout/layouts/layouts.hpp"


GridLayouts::GridLayouts(std::shared_ptr<Config> config) {
  sequencer = std::make_shared<GridLayout::Sequencer>(config);

  layout_by_name[sequencer->name] = sequencer;
}
