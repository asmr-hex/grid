#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/er1/er1.hpp"
#include "anemone/plugins/instruments/er1/controllers/midi.hpp"


ER1::MidiController::MidiController(std::shared_ptr<IO> io,
                                            std::shared_ptr<State> state,
                                            std::shared_ptr<ER1> er1)
{
  // stream for played back midi on/off events
  auto playback_midi_note_events = er1->playback_midi_events.get_observable()
    | rx::filter([er1] (midi_event_t e) {
                   if (e.data.size() == 0) return false;

                   // filter for 
                   return
                     is_midi_on_note_message(e.data) ||
                     is_midi_off_note_message(e.data);
                 })
    | rx::map([er1] (midi_event_t e) -> std::pair<std::shared_ptr<ER1::Pad>, bool> {
        // TODO handle error if the note corresponds to no pad...
        
        // maps the inbound midi note to the pad that it triggers and also
        // whether it is a midi on or off event
        auto on  = is_midi_on_note_message(e.data);
        auto pad = er1->midi_map.note_to_pad.at(e.data[1]);

        return { pad, on };
      });

  // subscribe to midi note events
  playback_midi_note_events
    .subscribe([er1] (std::pair<std::shared_ptr<ER1::Pad>, bool> p) {
                 auto pad = std::get<0>(p);
                 auto on  = std::get<1>(p);

                 // set playback state of this pad.
                 pad->is_playing.get_subscriber().on_next(on);
               });
}
