/**
 * @file   io/midi/device/factory.hpp
 * @brief  Midi Device Factory Template
 * @author coco
 * @date   2020-01-25
 *************************************************/


#ifndef ANEMONE_IO_MIDI_DEVICE_FACTORY_H
#define ANEMONE_IO_MIDI_DEVICE_FACTORY_H

#include <memory>
#include <type_traits>

#include "anemone/io/midi/device/midi.hpp"

/// @brief Interface for a midi device factory.
///
class MidiDeviceFactory {
public:
  virtual ~MidiDeviceFactory() = default;

  virtual std::shared_ptr<MidiDevice> make() = 0;
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
template<typename T>
class MidiDeviceFactoryFor : public MidiDeviceFactory {
  static_assert(std::is_base_of<MidiDevice,T>::value);
public:
  
  virtual std::shared_ptr<MidiDevice> make() override {
    return std::make_shared<T>();
  }
};

#endif
