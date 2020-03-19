#include "anemone/state/instruments/er1/er1.hpp"


ER1::ER1(std::shared_ptr<Config> config)
  : Instrument(create_instrument(InstrumentName::ER1, config)),
    layout(std::make_shared<ER1Layout>(config))
{}
