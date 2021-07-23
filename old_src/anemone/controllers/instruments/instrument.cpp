#include "anemone/controllers/instruments/instrument.hpp"


InstrumentController::InstrumentController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  // TODO we need information to filter on
  events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == "Shift" &&
                     e.type    == GridEvent::Pressed;
                 });
}
