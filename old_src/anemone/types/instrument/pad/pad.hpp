/**
 * @file   types/instruments/pad/pad.hpp
 * @brief  Instrument Pad Class Type
 * @author coco
 * @date   2020-04-14
 *************************************************/

#ifndef TYPES_INSTRUMENTS_PAD_PAD_H
#define TYPES_INSTRUMENTS_PAD_PAD_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/types/io/midi/data.hpp"
#include "anemone/types/io/midi/event.hpp"
#include "anemone/types/instrument/parameter/parameter.hpp"


class Pad {
public:
  /// @brief is the pad selected?
  rx::behavior<bool> is_selected = rx::behavior<bool>(false);
  
  /// @brief is the pad playing?
  rx::behavior<bool> is_playing = rx::behavior<bool>(false);
  
  struct {
    /// @brief the output channel for midi messages.
    midi_channel_t channel;
      
    /// @brief scientific pitch notation representation of midi note which this pad triggers.
    midi_spn_t spn;

    /// @brief midi note number which this pad triggers.
    midi_note_number_t note_number;

    /// @brief the note on data to be emitted when this pad is triggered.
    midi_data_t note_on_data;

    /// @brief the note off data to be emitted when this pad is untriggered.
    midi_data_t note_off_data;
        
  } midi;

  /// @brief pad specific parameters.
  std::vector< std::shared_ptr<Parameter> > parameters;
      
  Pad(midi_spn_t, midi_channel_t);
};

#endif
