#include "anemone/controllers/play_pause.hpp"


PlayPauseController::PlayPauseController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  auto press_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == "PlayPause" &&
                     e.type    == GridEvent::Pressed;
                 });

  // auto unpress_events = io->grid_events
  //   | rx::filter([] (grid_event_t e) {
  //                  return
  //                    e.section == "PlayPause" &&
  //                    e.type    == GridEvent::Unpressed;
  //                });
    
  press_events.subscribe([state] (grid_event_t e) {
                        auto rendered_instrument = state->instruments->rendered.get_value();
                        auto rendered_part = rendered_instrument->status.part.under_edit.get_value();
                        
                        auto rendered_part_is_playing = rendered_part->transport.is_playing.get_value();
                        auto rendered_part_is_paused = rendered_part->transport.is_paused.get_value();
                        auto rendered_part_is_stopped = rendered_part->transport.is_stopped.get_value();

                        if (rendered_part_is_playing) {
                          // lets pause
                          rendered_part->transport.is_paused.get_subscriber().on_next(true);
                          rendered_part->transport.is_playing.get_subscriber().on_next(false);
                          rendered_instrument->status.is_playing.get_subscriber().on_next(false);
                        } else if (rendered_part_is_paused) {
                          // TODO lets prepare to unpause

                          // for nnow lets just unpause
                          rendered_part->transport.is_paused.get_subscriber().on_next(false);
                          rendered_part->transport.is_playing.get_subscriber().on_next(true);
                          rendered_instrument->status.is_playing.get_subscriber().on_next(true);
                        } else {
                          // lets play
                          rendered_part->transport.is_playing.get_subscriber().on_next(true);
                          rendered_instrument->status.is_playing.get_subscriber().on_next(true);
                        }
                      });
  // off_events.subscribe([state] (grid_event_t e) {
  //                        state->controls->set_shift(false);
  //                      });
}
