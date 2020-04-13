#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/er1/er1.hpp"
#include "anemone/plugins/instruments/er1/controllers/pads.hpp"


ER1::PadsController::PadsController(std::shared_ptr<IO> io,
                                    std::shared_ptr<State> state,
                                    std::shared_ptr<ER1> er1)
{
  // stream of oscillator pad events
  auto osc_pad_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.layout  == LayoutName::ER1              &&
                     e.section == GridSectionName::Oscillators;
                 });

  // stream of cymbal pad events
  auto cymbal_pad_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.layout  == LayoutName::ER1              &&
                     e.section == GridSectionName::Cymbals;
                 });

  // stream of audio in pad events
  auto audio_in_pad_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.layout  == LayoutName::ER1              &&
                     e.section == GridSectionName::AudioIns;
                 });
    
  // subscribe to oscillator pad events
  osc_pad_events
    .subscribe([io, er1] (grid_event_t e) {
                 auto pressed = e.type == GridEvent::Pressed ? true : false;
                 auto osc     = er1->pads.oscillators[e.index];

                 // construct midi event to emit
                 midi_event_t midi_event =
                   { .source      = "",
                     .destination = "",
                     .data        = pressed ? osc->midi.note_on_data : osc->midi.note_off_data,
                   };

                 // emit the midi event
                 io->midi->emit(midi_event);

                 // if this was a press event (aka a midi note ON event was emitted), update
                 // the last midi note played
                 if (pressed) er1->update_last_midi_notes_played(midi_event);

                 // update the playback status of this pad
                 osc->is_playing.get_subscriber().on_next(pressed);
               });

  // subscribe to cymbal pad events
  cymbal_pad_events
    .subscribe([io, er1] (grid_event_t e) {
                 auto pressed = e.type == GridEvent::Pressed ? true : false;
                 auto cymbals = er1->pads.cymbals[e.index];

                 // construct midi event to emit
                 midi_event_t midi_event =
                   { .source      = "",
                     .destination = "",
                     .data        = pressed ? cymbals->midi.note_on_data : cymbals->midi.note_off_data,
                   };

                 // emit the midi event
                 io->midi->emit(midi_event);

                 // if this was a press event (aka a midi note ON event was emitted), update
                 // the last midi note played
                 if (pressed) er1->update_last_midi_notes_played(midi_event);

                 // update the playback status of this pad
                 cymbals->is_playing.get_subscriber().on_next(pressed);
               });

  // subscribe to oscillator pad events
  audio_in_pad_events
    .subscribe([io, er1] (grid_event_t e) {
                 auto pressed  = e.type == GridEvent::Pressed ? true : false;
                 auto audio_in = er1->pads.audio_ins[e.index];

                 // construct midi event to emit
                 midi_event_t midi_event =
                   { .source      = "",
                     .destination = "",
                     .data        = pressed ? audio_in->midi.note_on_data : audio_in->midi.note_off_data,
                   };

                 // emit the midi event
                 io->midi->emit(midi_event);

                 // if this was a press event (aka a midi note ON event was emitted), update
                 // the last midi note played
                 if (pressed) er1->update_last_midi_notes_played(midi_event);

                 // update the playback status of this pad
                 audio_in->is_playing.get_subscriber().on_next(pressed);
               });
}
