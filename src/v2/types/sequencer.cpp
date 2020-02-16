#include "anemone/types/sequencer.hpp"
#include "anemone/types/midi.hpp"


types::step::event::id_t types::step::event::make_id(types::step::event::protocol_t protocol,
                                                     types::midi::data_t data) {
  switch (protocol) {
  case protocol_t::Midi:
  default: // TODO remove this default!
    return (types::step::event::id_t)(((0x0000 | data[0]) << 4) & data[1]);
  }
}

types::step::event_t types::step::midi_note_on(std::string note,
                                               unsigned int channel,
                                               unsigned int velocity) {
  auto protocol = types::step::event::protocol_t::Midi;
  auto data     = types::midi::note_on(note, channel, velocity);
  auto id       = types::step::event::make_id(protocol, data);

  return {
          .protocol = protocol,
          .id       = id,
          .data     = data,
  };
}
