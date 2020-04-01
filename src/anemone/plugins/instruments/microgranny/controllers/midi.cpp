#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/microgranny/microgranny.hpp"
#include "anemone/plugins/instruments/microgranny/controllers/midi.hpp"


MicroGranny::MidiController::MidiController(std::shared_ptr<IO> io,
                                            std::shared_ptr<State> state,
                                            std::shared_ptr<MicroGranny> microgranny)
{
  // // all incoming midi on notes if this instrument is active
  // auto midi_on_notes = io->midi_events
  //   | rx::filter([state, microgranny] (midi_event_t e) {
  //                  return
  //                    is_midi_on_note_message(e.data) &&
  //                    state->instruments->rendered.get_value()->name == microgranny->name;
  //                });

  // // all incoming midi off notes if this instrument is active
  // auto midi_off_notes = io->midi_events
  //   | rx::filter([state, microgranny] (midi_event_t e) {
  //                  return
  //                    is_midi_off_note_message(e.data) &&
  //                    state->instruments->rendered.get_value()->name == microgranny->name;
  //                });

  // // all incoming midi cc messages if this instrument is active
  // auto midi_cc = io->midi_events
  //   | rx::filter([state, microgranny] (midi_event_t e) {
  //                  return
  //                    is_midi_cc_message(e.data) &&
  //                    state->instruments->rendered.get_value()->name == microgranny->name;
  //                });

  // midi on note events being played by the sequencer
  auto played_back_pads_on = microgranny->playback_midi_events.get_observable()
    | rx::filter([] (midi_event_t e) {
                   if (e.data.size() == 0) return false;
                   
                   // filter for midi on notes only
                   return is_midi_on_note_message(e.data);
                 })
    | rx::map([microgranny] (midi_event_t e) {
                // convert midi note number to pad index
                int pad_idx;
                
                // TODO hmmm, do we have to always loop over the pads?
                // more optimized would be to keep a mapping....
                for (pad_idx = 0; pad_idx < microgranny->midi_map.notes.size(); pad_idx++) {
                  if (spn_to_num(microgranny->midi_map.notes[pad_idx]) == (unsigned int)e.data[1])
                    return pad_idx;
                }

                return -1;
              })
    | rx::filter([] (int pad) { return pad >= 0; });

  // midi off note events being played by the sequencer
  auto played_back_pads_off = microgranny->playback_midi_events.get_observable()
    | rx::filter([] (midi_event_t e) {
                   if (e.data.size() == 0) return false;
                   
                   // filter for midi on notes only
                   return is_midi_off_note_message(e.data);
                 })
    | rx::map([microgranny] (midi_event_t e) {
                // convert midi note number to pad index
                int pad_idx;

                // TODO hmmm, do we have to always loop over the pads?
                // more optimized would be to keep a mapping....
                for (pad_idx = 0; pad_idx < microgranny->midi_map.notes.size(); pad_idx++) {
                  if (spn_to_num(microgranny->midi_map.notes[pad_idx]) == (unsigned int)e.data[1])
                    return pad_idx;
                }

                return -1;
              })
    | rx::filter([] (int pad) { return pad >= 0; });


  
  // ============ SUBSCRIBE ================


  // subscribe to played back notes
  played_back_pads_on
    .subscribe([microgranny] (unsigned int pad) {
                 microgranny->pad_is_playing[pad].get_subscriber().on_next(true);
               });
  played_back_pads_off
    .subscribe([microgranny] (unsigned int pad) {
                 microgranny->pad_is_playing[pad].get_subscriber().on_next(false);
               });
}
