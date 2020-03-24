#include <locale>

#include "anemone/types/io/midi/data.hpp"


midi_note_number_t spn_to_num(midi_spn_t spn) {
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
  // TODO we aren't parsing negative octaves here... in the official
  // midi spec, there is a -1 octave.
  // officially C0 has the midi note number 12?
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
  
  return (midi_note_number_t)(((octave + 1) * 12) + note);
}

midi_data_t midi_note_on(midi_spn_t note, unsigned int channel, unsigned int velocity) {
  std::vector<unsigned char> data;
  data.push_back((unsigned char)(143 + channel));
  data.push_back((unsigned char)spn_to_num(note));
  data.push_back((unsigned char)velocity);

  return data;
}

midi_data_t midi_note_off(midi_spn_t note, unsigned int channel) {
  std::vector<unsigned char> data;
  data.push_back((unsigned char)(127 + channel));
  data.push_back((unsigned char)spn_to_num(note));
  data.push_back(0);

  return data;
}    
