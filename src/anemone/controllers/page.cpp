#include "anemone/controllers/page.hpp"


PageController::PageController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  auto on_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == "Pages" &&
                     e.type    == GridEvent::Pressed;
                 });
  
  on_events.subscribe([state] (grid_event_t e) {
                        auto rendered_instrument = state->instruments->rendered.get_value();
                        auto rendered_part = rendered_instrument->status.part.under_edit.get_value();
                        
                        rendered_part->page.rendered.get_subscriber().on_next(e.index);
                        rendered_part->page.under_edit.get_subscriber().on_next(e.index);
                      });
}
