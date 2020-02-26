#include "anemone/state/instruments.hpp"


Instruments::Instruments(std::shared_ptr<Config> config)
  : er1(std::make_shared<ER1>(config)),
    rendered(rx::behavior<std::shared_ptr<Instrument> >(er1))
{
  by_name[er1->name] = er1;
}

void Instruments::render(InstrumentName name) {
  rendered.get_subscriber().on_next(by_name[name]);
}

void Instruments::advance_step() {
  for (auto itr : by_name) {
    auto instrument = itr.second;
    if (!instrument->is_playing) continue;

    auto part = instrument->status.part.in_playback.get_value();

    // get midi data at this step
    auto step_events = part->advance_step();

    for (auto step_event : step_events) {
      // output step events
      midi_output.get_subscriber().on_next(step_event);
    }
  }
}
