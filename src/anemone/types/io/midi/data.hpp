/**
 * @file   types/io/midi/data.hpp
 * @brief  Midi Data Related Types & Helpers
 * @author coco
 * @date   2020-01-22
 *************************************************/


#ifndef ANEMONE_TYPES_IO_MIDI_DATA_H
#define ANEMONE_TYPES_IO_MIDI_DATA_H

#include <string>
#include <vector>


/// @brief midi data type.
typedef std::vector<unsigned char> midi_data_t;

/// @brief midi note type definition.
typedef unsigned int midi_note_number_t;

/// @brief midi scientific pitch notation typedef.
typedef std::string midi_spn_t;

/// @brief translate 'scientific pitch notation' to note number.
midi_note_number_t spn_to_num(midi_spn_t spn);

/// @brief create a midi on note given an spn note.
midi_data_t midi_note_on(midi_spn_t note, unsigned int channel, unsigned int velocity);

/// @brief create a midi off note given an spn note.
midi_data_t midi_note_off(midi_spn_t note, unsigned int channel);


#endif
