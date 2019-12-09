#ifndef IO_MIDI_OBSERVER_H
#define IO_MIDI_OBSERVER_H

#include "types.hpp"


class MidiObserver {
public:
  virtual void handle(midi_event_t) = 0;
};

#endif
