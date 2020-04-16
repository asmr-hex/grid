#include "anemone/controllers/shift.hpp"


ShiftController::ShiftController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  auto on_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == "Shift" &&
                     e.type    == GridEvent::Pressed;
                 });

  auto off_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == "Shift" &&
                     e.type    == GridEvent::Unpressed;
                 });
    
  on_events.subscribe([state] (grid_event_t e) {
                        state->controls->set_shift(true);
                      });
  off_events.subscribe([state] (grid_event_t e) {
                         state->controls->set_shift(false);
                       });
}
