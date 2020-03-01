#include "anemone/state/instruments/part.hpp"


Part::Part(part_idx_t id)
  : id(id)
{}


std::vector<step_event_t> Part::advance_step() {
  // get the midi events at the current step
  auto step_events = sequence.get_events_at();
  
  // advance the step
}
