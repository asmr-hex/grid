#include <spdlog/spdlog.h>

#include "anemone/types/instrument/parameter/parameter.hpp"


Parameter::Parameter(std::string name)
  : name(name)
{}

bool Parameter::is_mapped_to(midi_event_t event) {
  // TODO currently, this only detects cc messages...eventually allow mappings
  // from other types of midi message types.
  return
    event.source  == midi_map.source      &&
    event.data[0] == midi_map.status_byte &&
    event.data[1] == midi_map.control;
}

void Parameter::set_current_value(midi_event_t event) {
  // extract cc value from event
  value.current.get_subscriber().on_next(event.data[2]);
}

std::vector<midi_event_t> Parameter::get_midi_output() {
  return convert_value_to_midi_events(value.current.get_value());
}
