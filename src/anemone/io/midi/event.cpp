#include "anemone/io/midi/event.hpp"


midi_event_t::midi_event_t(std::string source, double timestamp, const std::vector<unsigned char> *data)
  : source(source), timestamp(timestamp), data(data) {}
