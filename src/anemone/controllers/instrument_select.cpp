#include "anemone/controllers/instrument_select.hpp"


InstrumentSelectController::InstrumentSelectController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  auto selection_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == GridSectionName::InstrumentSelect &&
                     e.type    == GridEvent::Pressed;
                 })
    | rx::map([] (grid_event_t e) {
                return e.index;
              });

  selection_events
    .subscribe([state] (unsigned int idx) {
                 auto selected_instrument = state->instruments->by_index[idx];
                 state->instruments->render(selected_instrument->name);
               });
}
