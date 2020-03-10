/**
 * @file   types/io/midi/event.hpp
 * @brief  Midi Event Related Types & Helpers
 * @author coco
 * @date   2020-03-07
 *************************************************/


#ifndef ANEMONE_TYPES_IO_MIDI_EVENT_H
#define ANEMONE_TYPES_IO_MIDI_EVENT_H


#include "anemone/types/io/midi/data.hpp"


class midi_event_t {
public:
  std::string source;
  std::string destination;
  midi_data_t data;
};


#endif
