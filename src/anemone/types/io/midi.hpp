/**
 * @file   types/io/midi.hpp
 * @brief  Midi Related Types & Helpers
 * @author coco
 * @date   2020-01-22
 *************************************************/


#ifndef ANEMONE_TYPES_IO_MIDI_H
#define ANEMONE_TYPES_IO_MIDI_H

#include <string>
#include <vector>


/// @brief midi data type.
typedef std::vector<unsigned char> midi_data_t;

/// @brief translate 'scientific pitch notation' to note number.
unsigned int spn_to_num(std::string spn);

/// @brief create a midi on note given an spn note.
midi_data_t midi_note_on(std::string note, unsigned int channel, unsigned int velocity);

/// @brief create a midi off note given an spn note.
midi_data_t midi_note_off(std::string note, unsigned int channel);


#endif
