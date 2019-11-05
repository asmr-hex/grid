#ifndef OUTPUT_H
#define OUTPUT_H

#include <monome.h>
#include <RtMidi.h>


struct Output {
  monome_t *monome;
  RtMidiOut *midi;
};

#endif
