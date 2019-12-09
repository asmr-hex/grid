#ifndef IO_MIDI_H
#define IO_MIDI_H

#include <string>

#include "observer.hpp"


class Midi {
public:
  virtual void connect_midi_in(std::string port_name) = 0;
  virtual void connect_midi_out(std::string port_name) = 0;

  virtual void get_midi_in_port_names() = 0;
  virtual void get_midi_out_port_names() = 0;
  
  virtual void register_observer(MidiObserver *) = 0;
  virtual void listen() = 0;

  // TODO function for midi emit messages
};

#endif
