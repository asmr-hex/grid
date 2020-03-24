/**
 * @file   io/midi/device/midi.hpp
 * @brief  Midi Device Abstract Classes
 * @author coco
 * @date   2020-03-07
 *************************************************/


#ifndef ANEMONE_IO_MIDI_DEVICE_MIDI_H
#define ANEMONE_IO_MIDI_DEVICE_MIDI_H


#include <map>
#include <string>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"


class MidiDevice {
public:
  virtual ~MidiDevice() = default;

  /// @brief connects to a midi device.
  ///
  /// @param name   the name of the midi device.
  ///
  virtual void connect() = 0;

  /// @brief list_devices generates a map of detected device names to port number.
  ///
  /// @return devices   a map from device name to device port.
  ///
  virtual std::map<std::string, unsigned int> list_devices() = 0;

  /// @brief returns the name of the midi device
  ///
  /// @return name   the name of the midi device.
  ///
  virtual std::string name() = 0;
};


class MidiInputDevice : public MidiDevice {
public:
  /// @brief listens to incoming midi messages.
  virtual void listen() = 0;
};


class MidiOutputDevice : public MidiDevice {
public:
  /// @brief emits midi messages to the midi output.
  virtual void emit(midi_data_t) = 0;
};

#endif
