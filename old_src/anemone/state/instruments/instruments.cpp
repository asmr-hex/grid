#include "anemone/plugins.hpp"
#include "anemone/state/instruments/instruments.hpp"


// TODO this issue currently seems to be with how these instruments are initialized! 
Instruments::Instruments(std::shared_ptr<Config> config, std::shared_ptr<InstrumentPluginManager> plugin_manager)
  : rendered(rx::behavior<std::shared_ptr<Instrument> >(plugin_manager->default_instrument))
{
  // add instruments from plugin manager
  unsigned int idx = 0;
  for (auto instrument : plugin_manager->instruments) {
    by_name[instrument->name] = instrument;
    by_index[idx] = instrument;
    idx++;
  }
}

void Instruments::render(InstrumentName name) {
  rendered.get_subscriber().on_next(by_name[name]);
}
