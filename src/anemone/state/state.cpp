#include "anemone/state/state.hpp"


State::State(std::shared_ptr<Config> config)
  : layouts(std::make_shared<GridLayouts>(config)),
    controls(std::make_shared<GlobalControls>(config)),
    instruments(std::make_shared<Instruments>(create_instruments(config)))
{}
