#ifndef IO_MIDI_DEVICE_RTMIDI_H
#define IO_MIDI_DEVICE_RTMIDI_H

#include <map>
#include <string>
#include <memory>

#include <RtMidi.h>

#include "anemone/io/midi/event.hpp"
#include "anemone/io/midi/device/midi.hpp"


class RTMidi : public MidiDevice {
public:
  RTMidi();

  virtual void connect_in(std::string port_name) override;
  virtual void connect_out(std::string port_name) override;

  virtual midi_ports_t get_port_names() override;

  virtual void listen() override;
  virtual void emit(midi_event_t) override;

private:
  std::unique_ptr<RtMidiOut> out;
  std::unique_ptr<RtMidiIn> in;

  midi_ports_t ports;

  std::map<std::string, unsigned int> get_output_port_names();
  std::map<std::string, unsigned int> get_input_port_names();

  static void callback_wrapper(double deltatime, std::vector<unsigned char> *msg, void *user_data);
  static midi_event_t transform(double deltatime, std::vector<unsigned char> *msg);
};

#endif
