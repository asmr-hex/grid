#ifndef SEQUENCER_UTILS_H
#define SEQUENCER_UTILS_H

#include <string>
#include <locale>

#include "types.hpp"


step_event_t turn_led_off(unsigned int x, unsigned int y) {
  return {
          .protocol = OSC,
          .id = (unsigned short)(0x0100 | ( x << 4 ) | y ),
  };
};

step_event_t turn_led_on(unsigned int x, unsigned int y) {
  return {
          .protocol = OSC,
          .id = (unsigned short)(0x0000 | ( x << 4 ) | y),
  };
};

namespace Midi {
  unsigned int spn_to_num(std::string spn) {
    int octave, note;
    int octave_num_offset = 1;
    std::locale loc;

    int note_ascii_num = (int)std::tolower(spn[0], loc);
    switch (note_ascii_num) {
    case 97:
      // this is an 'a'
      note = 9; 
      break;
    case 98:
      // this is a 'b'
      note = 11;
      break;
    case 99:
      // this is a 'c'
      note = 0;
      break;
    case 100:
      // this is a 'd'
      note = 2;
      break;
    case 101:
      // this is a 'e'
      note = 4;
      break;
    case 102:
      // this is a 'f'
      note = 5;
      break;
    case 103:
      // this is a 'g'
      note = 7;
      break;
    }

    // detect sharps/flats
    if (spn[1] == 'b') {
      note--;
      octave_num_offset++;
    } else if (spn[1] == '#') {
      note++;
      octave_num_offset++;
    }

    // parse octave
    octave = stoi(spn.substr(octave_num_offset));

    // adjust for sharps/flats causing octave change.
    switch (note) {
    case -1:
      // we've gone down an octave and are at a 'b'
      note = 11;
      octave--;
      break;
    case 12:
      // we've gone up an octave and are at 'c'
      note = 0;
      octave++;
      break;
    }
    
    return (unsigned int)((octave * 12) + note);
  };
}

step_event_t midi_note_on(std::string note, unsigned int channel, unsigned int velocity) {
  std::vector<unsigned char> data;
  data.push_back((unsigned char)(144 + channel));
  data.push_back((unsigned char)Midi::spn_to_num(note));
  data.push_back((unsigned char)velocity);
  
  return {
          .protocol = MIDI,
          .id = (unsigned short)(((0x0000 | data[0]) << 4) & data[1]),
          .data = data,
  };
};

step_event_t midi_note_off(std::string note, unsigned int channel) {
  std::vector<unsigned char> data;
  data.push_back((unsigned char)(128 + channel));
  data.push_back((unsigned char)Midi::spn_to_num(note));
  data.push_back(0);
  
  return {
          .protocol = MIDI,
          .id = (unsigned short)(((0x0000 | data[0]) << 4) & data[1]),
          .data = data,
  };
};

step_event_t midi_note_off_from_on_event(step_event_t on_event) {
  std::vector<unsigned char> data;
  data.push_back((unsigned char)(on_event.data[0] - 16));
  data.push_back(on_event.data[1]);
  data.push_back(on_event.data[2]);
  
  return {
          .protocol = MIDI,
          .id = (unsigned short)(((0x0000 | data[0]) << 4) & data[1]),
          .data = data,
  };
};

event_uid_t midi_off_id_from_on_id(event_uid_t on_id) {
  return (((on_id >> 4) - 16) << 4) | (0x00FF & on_id);
};

step_idx_t get_fine_step_index(unsigned int coarse_step_index) {
  return coarse_step_index * constants::PPQN_MAX;  
};

#endif
