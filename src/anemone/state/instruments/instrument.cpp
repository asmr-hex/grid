#include "anemone/state/instruments/instrument.hpp"


Instrument::Instrument(InstrumentName name, std::shared_ptr<Config> config)
  : name(name),
    status({ .part = { .in_playback = rx::behavior<std::shared_ptr<Part> >(std::make_shared<Part>(0)),
                       .under_edit  = rx::behavior<std::shared_ptr<Part> >(std::make_shared<Part>(0))},
             .bank                  = {},
             .is_playing            = rx::behavior<bool>(true),
             .stop_on_next_measure  = rx::behavior<bool>(false)
      })
{
  // populate new parts TODO make this connfigurable....
  for (unsigned int i = 0; i < 49; ++i) {
    parts.push_back(std::make_shared<Part>(i));
  }
}
