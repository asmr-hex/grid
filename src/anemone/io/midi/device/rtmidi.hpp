/**
 * @file   io/midi/device/rtmidi.hpp
 * @brief  Midi RTMidi Device Class
 * @author coco
 * @date   2020-03-07
 *************************************************/


#ifndef ANEMONE_IO_MIDI_DEVICE_RTMIDI_H
#define ANEMONE_IO_MIDI_DEVICE_RTMIDI_H


#include <map>
#include <string>
#include <thread>
#include <chrono>

#include <RtMidi.h>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"

#include "anemone/io/midi/device/midi.hpp"


/// @brief Class for midi input.
class RTMidiIn : public MidiInputDevice {
public:
  /// @brief constructs an input midi device.
  ///
  /// @param input_stream   an rx subscriber for adding midi in events to the input msg stream.
  ///
  RTMidiIn(std::string, rx::subscriber<midi_event_t>);

  /// @brief connects to the midi input device with retries.
  ///
  /// @description this methods blocks while it attempts to connect to the
  /// input device.
  ///
  /// @param name   the device name.
  ///
  virtual void connect() override;

  /// @brief list_devices generates a map of detected device names to port number.
  ///
  /// @return devices   a map from device name to device port.
  ///
  virtual std::map<std::string, unsigned int> list_devices() override;
  
  /// @brief listens to incoming midi messages.
  virtual void listen() override;

  /// @returns the name of the midi device.
  virtual std::string name() override;
  
  /// @brief the midi input message stream.
  rx::subscriber<midi_event_t> input_stream;

private:
  /// @brief midi in device name.
  std::string device_name;
  
  std::unique_ptr<RtMidiIn> input;
};


/// @brief Class for midi output.
class RTMidiOut : public MidiOutputDevice {
public:
  RTMidiOut(std::string);
  
  /// @brief connects to the midi output device with retries.
  ///
  /// @description this methods blocks while it attempts to connect to the
  /// output device.
  ///
  /// @param name   the device name.
  ///
  virtual void connect() override;

  /// @brief list_devices generates a map of detected device names to port number.
  ///
  /// @return devices   a map from device name to device port.
  ///
  virtual std::map<std::string, unsigned int> list_devices() override;

  /// @brief emits midi messages to the midi output.
  virtual void emit(midi_event_t) override;

  /// @returns the name of the midi device.
  virtual std::string name() override;
  
private:
  /// @brief midi out device name.
  std::string device_name;
  
  std::unique_ptr<RtMidiOut> output;
};

#endif
