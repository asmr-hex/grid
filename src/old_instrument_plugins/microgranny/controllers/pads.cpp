#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/microgranny/microgranny.hpp"
#include "anemone/plugins/instruments/microgranny/controllers/pads.hpp"


MicroGranny::PadsController::PadsController(std::shared_ptr<IO> io,
                                            std::shared_ptr<State> state,
                                            std::shared_ptr<MicroGranny> microgranny)
{
  // pad press events (grid)
  auto pad_press_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.layout  == "MicroGranny"    &&
                     e.section == "MicroGrannySamples"   &&
                     e.type    == GridEvent::Pressed;
                 })
    | rx::map([] (grid_event_t e) {
                return e.index;
              });

  // pad unpress events (grid)
  auto pad_unpress_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.layout  == "MicroGranny"    &&
                     e.section == "MicroGrannySamples"   &&
                     e.type    == GridEvent::Unpressed;
                 })
    | rx::map([] (grid_event_t e) {
                return e.index;
              });


  // subscribe to grid press events
  pad_press_events
    .subscribe([io, microgranny] (grid_section_index_t pad) {

                 // create midi note to emit
                 midi_event_t midi_event = { .source      = "",
                                             .destination = "",
                                             .data        = midi_note_on(microgranny->midi_map.notes[pad],
                                                                         microgranny->midi_map.channel,
                                                                         127),
                 };

                 // emit pad midi note on
                 io->midi->emit(midi_event);

                 microgranny->update_last_midi_notes_played(midi_event);
                 microgranny->pad_is_playing[pad].get_subscriber().on_next(true);
               });

  // subscribe to grid unpress events
  pad_unpress_events
    .subscribe([io, microgranny] (grid_section_index_t pad) {
                 // emit midi off note
                 io->midi->emit({ .source      = "",
                                  .destination = "",
                                  .data        = midi_note_off(microgranny->midi_map.notes[pad],
                                                               microgranny->midi_map.channel),
                   });

                 microgranny->pad_is_playing[pad].get_subscriber().on_next(false);
               });
}
