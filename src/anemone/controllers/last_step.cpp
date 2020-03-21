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
                        auto shift = state->controls->shift.get_value();
                        auto follow_cursor = rendered_part->page.follow_cursor;
                        
                        if (shift) {
                          
                          if (follow_cursor.get_value()) {
                            // turn off follow cursor
                            follow_cursor.get_subscriber().on_next(false);
                          } else {
                            // turn on follow cursor
                            follow_cursor.get_subscriber().on_next(true);

                            // make playing, rendered, and under edit page equal
                            auto page_size = state->layouts->sequencer->steps->size();
                            auto page_in_playback = granular_to_paged_step(rendered_part->step.current.get_value(), page_size).page;
                            rendered_part->page.under_edit.get_subscriber().on_next(page_in_playback);
                            rendered_part->page.rendered.get_subscriber().on_next(page_in_playback);
                          }
                        } else {
                          // make the last page visible
                          rendered_part->page.rendered.get_subscriber().on_next(last_page);

                          // make show last step true
                          rendered_part->step.show_last.get_subscriber().on_next(true);  
                        }
                      });

  last_step_unpress_events.subscribe([state] (grid_event_t e) {
                                       auto rendered_instrument = state->instruments->rendered.get_value();
                                       auto rendered_part = rendered_instrument->status.part.under_edit.get_value();
                                       auto page_under_edit = rendered_part->page.under_edit.get_value();
                                       auto shift = state->controls->shift.get_value();

                                       if (!shift) {
                                         // go back to page under edit as the rendered page.
                                         rendered_part->page.rendered.get_subscriber().on_next(page_under_edit);

                                         // make show last step false.
                                         rendered_part->step.show_last.get_subscriber().on_next(false); 
                                       }
                                     });
}
