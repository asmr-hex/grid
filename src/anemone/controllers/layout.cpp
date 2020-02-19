#include "anemone/controllers/layout.hpp"


LayoutController::LayoutController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  // auto layout_events = io->grid_events
  //   | rx::filter([] (grid_event_t e) {
  //                  return
  //                    e.section == GridSectionName::Shift &&
  //                    e.type    == GridEvent::Pressed;
  //                });
    
  //   layout_events.subscribe([state] (grid_event_t e) {
  //                             state->layout->use_layout(LayoutName::SequencerAndInstrument);
  //                           });
}
