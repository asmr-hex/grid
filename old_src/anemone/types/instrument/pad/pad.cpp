#include <spdlog/spdlog.h>

#include "anemone/types/instrument/pad/pad.hpp"


Pad::Pad(midi_spn_t spn, midi_channel_t chan)
  : midi({ .channel       = chan,
           .spn           = spn,
           .note_number   = spn_to_num(spn),
           .note_on_data  = midi_note_on(spn, chan, 127),
           .note_off_data = midi_note_off(spn, chan),
    })
{}
