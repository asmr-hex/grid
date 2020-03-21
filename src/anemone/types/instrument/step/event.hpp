/**
 * @file   types/instrument/step/event.hpp
 * @brief  Sequence Step Event related types.
 * @author coco
 * @date   2020-02-20
 *************************************************/


#ifndef ANEMONE_TYPES_INSTRUMENT_SEQUENCE_STEP_EVENT_H
#define ANEMONE_TYPES_INSTRUMENT_SEQUENCE_STEP_EVENT_H

#include <string>
#include <vector>

#include "anemone/types/io/midi/data.hpp"


// forward declaration. see anemone/types/midi.hpp
typedef std::vector<unsigned char> midi_data_t;

/// @brief Step event protocol.
enum class step_event_protocol_t {
                                  /// MIDI protocol.
                                  Midi,
                                  /// Non-Registered Parameter Number (NRPN).
                                  NRPN,
};

/// @brief Step event unique identifier.
///
/// @details
/// for midi events:
/// `0x<status byte><data 1 byte>`
/// where,
///   `status byte` is the command + channel
///   `data 1 byte` is a 0-127 value (such as pitch)
///
typedef unsigned short step_event_id_t;

/// @brief Step event type.
struct step_event_t {
  step_event_protocol_t protocol;
  step_event_id_t       id;
  midi_data_t           data;

  /// @brief step_event_t constructor, responsible for creating the step_event_id_t.
  step_event_t(step_event_protocol_t p, midi_data_t d) {
    protocol = p;
    data = d;
    
    switch (protocol) {
    case step_event_protocol_t::Midi:
    default: // TODO remove this default!
      id = (step_event_id_t)(((0x0000 | data[0]) << 4) & data[1]);
    }
  }
  
  bool operator <(const step_event_t &rhs) const {
    return id < rhs.id;
  };

  /// @brief create a midi note on step event.
  static step_event_t make_midi_note_on(std::string note, unsigned int channel, unsigned int velocity) {
    auto protocol = step_event_protocol_t::Midi;
    auto data     = midi_note_on(note, channel, velocity);

    return step_event_t(protocol, data);
  }
};


#endif
