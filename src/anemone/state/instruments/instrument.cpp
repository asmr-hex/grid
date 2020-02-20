#include "anemone/state/instruments/instrument.hpp"


Instrument::Instrument(InstrumentName name, std::shared_ptr<Config> config)
  : name(name)
{
  // populate new parts TODO make this connfigurable....
  for (unsigned int i = 0; i < 49; ++i) {
    parts.push_back(std::make_shared<Part>(i));
  }
}
