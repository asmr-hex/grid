#include "anemone/state/instruments/instruments.hpp"


// TODO this issue currently seems to be with how these instruments are initialized! 
Instruments::Instruments(std::shared_ptr<Config> config, std::shared_ptr<ER1> er1)
  : er1(er1),
    rendered(rx::behavior<std::shared_ptr<Instrument> >(er1))
{
  by_name[er1->name] = er1;
}

void Instruments::render(InstrumentName name) {
  rendered.get_subscriber().on_next(by_name[name]);
}

Instruments create_instruments(std::shared_ptr<Config> config) {
  return Instruments(config, std::make_shared<ER1>(config));
}
