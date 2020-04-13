/**
 * @file   io/midi/midi.hpp
 * @brief  Midi Router Class
 * @author coco
 * @date   2020-01-25
 *************************************************/


#ifndef ANEMONE_IO_MIDI_H
#define ANEMONE_IO_MIDI_H


#include <map>
#include <thread>
#include <string>
#include <vector>
#include <memory>

#include "anemone/rx.hpp"
#include "anemone/config.hpp"
#include "anemone/types.hpp"

#include "anemone/io/midi/device/midi.hpp"
#include "anemone/io/midi/device/factory.hpp"


class Midi {
public:
  Midi(std::shared_ptr<Config>, std::shared_ptr<MidiDeviceFactory>);

  /// @brief connects to midi devices and returns a stream of incoming midi events.
  rx::observable<midi_event_t> connect();
  
  /// @brief emits a midi event
  void emit(midi_event_t);

  /// @brief emits a vector of midi events.
  void emit(std::vector<midi_event_t>&);

  /// @brief emits a vector of midi events. TODO do we needs this....?
  void emit(std::vector<midi_event_t>&&);
private:
  /// @brief incoming events subject.
  rx::subject<midi_event_t> incoming_events;

  /// @brief midi device factory.
  std::shared_ptr<MidiDeviceFactory> device_factory;

  /// @brief map of midi input devices.
  std::map<std::string, std::shared_ptr<MidiInputDevice> > input_devices;

  /// @brief map of midi output devices.
  std::map<std::string, std::shared_ptr<MidiOutputDevice> > output_devices;

  void make_input_devices(std::vector<std::string>);
  void make_output_devices(std::vector<std::string>);
};

#endif
