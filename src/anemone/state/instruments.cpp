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
  auto playing_parts = rx::observable<>::iterate(by_name)
    | rx::map([] (std::pair<InstrumentName, std::shared_ptr<Instrument> > p) {
                return std::get<1>(p);
              })
    | rx::filter([] (std::shared_ptr<Instrument> i) {
                   // filter for only playing instruments
                   return i->is_playing;
                 })
    | rx::map([] (std::shared_ptr<Instrument> i) {
                // retrieve the parts in playback.
                return i->status.part.in_playback.get_value();
              })
    | rx::as_blocking();

    // advance steps of each part in playback.
    playing_parts
      .subscribe([this] (std::shared_ptr<Part> part) {
                   auto step_events = part->advance_step();
                   
                   for (auto step_event : step_events) {
                     midi_output.get_subscriber().on_next(step_event);
                   }
                 });
}
