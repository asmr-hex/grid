/**
 * @file   io/midi/device/factory.hpp
 * @brief  Midi Device Factory Class
 * @author coco
 * @date   2020-03-07
 *************************************************/


#ifndef ANEMONE_IO_MIDI_DEVICE_FACTORY_H
#define ANEMONE_IO_MIDI_DEVICE_FACTORY_H

#include <string>
#include <memory>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"

#include "anemone/io/midi/device/midi.hpp"


class MidiDeviceFactory {
public:
  virtual ~MidiDeviceFactory() = default;

  virtual std::shared_ptr<MidiInputDevice> make_input(std::string, rx::subscriber<midi_event_t>) = 0;
  virtual std::shared_ptr<MidiOutputDevice> make_output(std::string) = 0;  
};

/// @brief Template class for creating `MidiDevice`s
///
/// @details
/// This template class implements the `MidiDeviceFactory` interface for any
/// type derived from `MidiDevice`. This allows us to not have to re-implement
/// a concrete factory class for every concrete midi device type we implement.
/// Importantly, it implements the `MidiDeviceFactory` interface so that we are
/// given polymorphism by using a pointer to this templated type, which avoids
/// having to make the calling class a template class (and every other class above
/// it in the ownership hierarchy). This is important because the point of having
/// this factory is to support dependency injection into the entire program.
///
template<typename In, typename Out>
class MidiDeviceFactoryFor : public MidiDeviceFactory {
  static_assert(std::is_base_of<MidiInputDevice,In>::value);
  static_assert(std::is_base_of<MidiOutputDevice,Out>::value);
public:
  
  virtual std::shared_ptr<MidiInputDevice> make_input(std::string name, rx::subscriber<midi_event_t> s) override {
    return std::make_shared<In>(name, s);
  };

  virtual std::shared_ptr<MidiOutputDevice> make_output(std::string name) override {
    return std::make_shared<Out>(name);
  };
};


#endif
