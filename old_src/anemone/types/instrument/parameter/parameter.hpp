/**
 * @file   types/instruments/parameter/parameter.hpp
 * @brief  Instrument Parameter Class Type
 * @author coco
 * @date   2020-04-14
 *************************************************/

#ifndef TYPES_INSTRUMENTS_PARAMETER_PARAMETER_H
#define TYPES_INSTRUMENTS_PARAMETER_PARAMETER_H

#include <memory>
#include <functional>

#include "anemone/rx.hpp"
#include "anemone/types/io/midi/data.hpp"
#include "anemone/types/io/midi/event.hpp"


/// @brief Parameter is a controllable instrument parameter.
///
/// @details a Parameter is a variable instrument parameter. a Parameter can be one of
/// two value types: (1) Discrete (2) Continuous. a Parameter can be Mappable or not,
/// meaning it can be ....hmm should ever parameter be mappable?
///
class Parameter {
public:
  enum class ValueType { Discrete, Continuous };

  ValueType type = ValueType::Continuous;
  std::string name;

  struct {
    rx::behavior<unsigned int> current  = rx::behavior<unsigned int>(80);
    rx::behavior<unsigned int> playback = rx::behavior<unsigned int>(0);
  } value;

  struct {
    rx::behavior<bool> mapping_in_progress = rx::behavior<bool>(false);
    std::string  source;
    unsigned int status_byte;
    unsigned int control;
  } midi_map;

  /// @brief this function is provided for each instrument and is implemented according
  /// to the midi specification of the instrument to which this parameter belongs.
  ///
  /// @details since the midi-controllable parameters on an instrument can be controlled
  /// by any type of midi messages, inncludinng Non-Registered PArameter Numbers (NRPN),
  /// this function provides flexibility to map a scalar parameter value to the appropriate
  /// output midi messages needed to control the parameter on the instrument (according to
  /// the instrument's specific midi specification).
  ///
  std::function<std::vector<midi_event_t>(unsigned int)> convert_value_to_midi_events;

  /// @brief this function is used to translate an inbound midi event to a current value.
  /// it essentially just extracts the appropriate value from the inbound event. it can be
  /// thought of as the inverse to the provided `convert_value_to_midi_event` function in a
  /// way.
  ///
  /// @details since we want to be able to map a wide range of different inbound midi message
  /// types (cc, pitch wheel, etc.) to the parameter value, we will need to provide a way to
  /// extract the appropriate value from these different types of input messages.
  ///
  /// NOTE: for now we will be limited to CC messages.
  std::function<unsigned int(midi_event_t)> convert_midi_event_to_value;
  
  /// @brief determines whether a midi event maps to this parameter.
  bool is_mapped_to(midi_event_t);
  
  /// @brief takes an inbound midi event and extracts the new parameter value from it
  /// in order to update the current parameter value. this method is the inverse of the
  /// `get_midi_output` method which maps the current value to its appropriate output
  /// midi message(s).
  void set_current_value(midi_event_t);
  
  /// @brief generates output midi events for the current parameter value.
  ///
  /// @remark since the midi output could be NRPN messages, which consist of
  /// three consecutive midi events, we are returning a vector of midi events.
  std::vector<midi_event_t> get_midi_output();
  
  Parameter(std::string name);
};

#endif
