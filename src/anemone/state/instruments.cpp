#include "anemone/state/instruments.hpp"


Instruments::Instruments(std::shared_ptr<Config> config)
  : rendered(rx::behavior<std::shared_ptr<Instrument> >(er1)),
    er1(std::make_shared<ER1>(config))
{
  by_name[er1->name] = er1;
}

void Instruments::render(InstrumentName name) {
  rendered.get_subscriber().on_next(by_name[name]);
}
