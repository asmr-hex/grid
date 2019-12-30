#ifndef IO_MIDI_DEVICE_MIDI_H
#define IO_MIDI_DEVICE_MIDI_H

#include <string>
#include <vector>

#include "anemone/io/observable.hpp"
#include "anemone/io/midi/event.hpp"


struct midi_ports_t {
  std::map<std::string, unsigned int> in;
  std::map<std::string, unsigned int> out;
};


class MidiDevice : public Observable<midi_event_t> {
public:
  virtual ~MidiDevice() = default;

  virtual void connect_in(std::string port_name) = 0;
  virtual void connect_out(std::string port_name) = 0;

  virtual midi_ports_t get_port_names() = 0;
  
  virtual void listen() = 0;
  virtual void emit(midi_event_t) = 0;
};


#endif
