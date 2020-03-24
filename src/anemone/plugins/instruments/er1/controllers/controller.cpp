#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/er1/er1.hpp"
#include "anemone/plugins/instruments/er1/controllers/controller.hpp"


ER1::Controller::Controller(std::shared_ptr<IO> io,
                            std::shared_ptr<State> state,
                            std::shared_ptr<ER1> er1_state)
{  
  auto on_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.layout  == LayoutName::ER1       &&
                     e.section == GridSectionName::OSC1 &&
                     e.type    == GridEvent::Pressed;
                 });

  on_events.subscribe([io, state, er1_state] (grid_event_t e) {
                        // emit ocs1 midi on note
                        io->midi->emit({ .source      = "",
                                         .destination = "",
                                         .data        = midi_note_on(er1_state->midi_map.notes.osc1,
                                                                     er1_state->midi_map.channel,
                                                                     127),
                          });

                        er1_state->pad_is_playing.osc1.get_subscriber().on_next(true);
                      });

  auto off_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.layout  == LayoutName::ER1       &&
                     e.section == GridSectionName::OSC1 &&
                     e.type    == GridEvent::Unpressed;
                 });

  off_events.subscribe([io, state, er1_state] (grid_event_t e) {
                        // emit ocs1 midi on note
                        io->midi->emit({ .source      = "",
                                         .destination = "",
                                         .data        = midi_note_off(er1_state->midi_map.notes.osc1,
                                                                     er1_state->midi_map.channel),
                          });

                        er1_state->pad_is_playing.osc1.get_subscriber().on_next(false);
                      });
}
