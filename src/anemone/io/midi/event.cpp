#include "anemone/io/midi/event.hpp"


midi_event_t::midi_event_t(double timestamp, const std::vector<unsigned char> *data)
  : timestamp(timestamp), data(data) {}
