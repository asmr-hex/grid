#include <spdlog/spdlog.h>

#include "anemone/controllers/last_step.hpp"


LastStepController::LastStepController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  auto last_step_press_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == GridSectionName::LastStep &&
                     e.type    == GridEvent::Pressed;
                 });

  auto last_step_unpress_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == GridSectionName::LastStep &&
                     e.type    == GridEvent::Unpressed;
                 });
  
  last_step_press_events.subscribe([state] (grid_event_t e) {
                        auto rendered_instrument = state->instruments->rendered.get_value();
                        auto rendered_part = rendered_instrument->status.part.under_edit.get_value();
                        auto last_page = rendered_part->page.last.get_value();
                        
                        // make the last page visible
                        rendered_part->page.rendered.get_subscriber().on_next(last_page);

                        // make show last step true
                        rendered_part->step.show_last.get_subscriber().on_next(true);
                      });

  last_step_unpress_events.subscribe([state] (grid_event_t e) {
                                       auto rendered_instrument = state->instruments->rendered.get_value();
                                       auto rendered_part = rendered_instrument->status.part.under_edit.get_value();
                                       auto page_under_edit = rendered_part->page.under_edit.get_value();

                                       // go back to page under edit as the rendered page.
                                       rendered_part->page.rendered.get_subscriber().on_next(page_under_edit);

                                       // make show last step false.
                                       rendered_part->step.show_last.get_subscriber().on_next(false);
                                     });
}
