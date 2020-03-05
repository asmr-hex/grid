#include "anemone/state/instruments/instrument.hpp"


Instrument::Instrument(InstrumentName name,
                       std::shared_ptr<Config> config,
                       std::vector<std::shared_ptr<Part> > parts,
                       sequence_layer_t default_midi_notes)
  : name(name),
    status({ .part = { .in_playback = rx::behavior<std::shared_ptr<Part> >(parts[0]),
                       .under_edit  = rx::behavior<std::shared_ptr<Part> >(parts[0])},
             .bank = { .in_playback = rx::behavior<bank_idx_t>(0),
                       .under_edit  = rx::behavior<bank_idx_t>(0)},
             .is_playing            = rx::behavior<bool>(true),
             .stop_on_next_measure  = rx::behavior<bool>(false)
      }),
    parts(parts),
    last_midi_notes_played(rx::behavior<sequence_layer_t>(default_midi_notes))
{}


Instrument create_instrument(InstrumentName name, std::shared_ptr<Config> config) {
  std::vector<std::shared_ptr<Part> > parts;
  
  // populate new parts TODO make this connfigurable....
  for (unsigned int i = 0; i < 49; ++i) {
    parts.push_back(std::make_shared<Part>(i));
  }

  // TODO make default midi notes configurable
  auto default_note = step_event_t::make_midi_note_on("c4", 1, 127);
  sequence_layer_t default_notes = { {default_note.id, default_note } };
  
  return Instrument(name, config, parts, default_notes);
}
