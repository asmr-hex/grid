#include "anemone/state/state.hpp"


State::State(std::shared_ptr<Config> config)
  : layout(std::make_shared<LayoutContext>(std::make_shared<GridLayouts>(config)))
{}
