#ifndef INPUT_H
#define INPUT_H

#include <monome.h>
#include <RtMidi.h>


struct Input {
  monome_t *monome;
  RtMidiIn *midi;
};

#endif
