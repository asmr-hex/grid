#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <map>
#include <vector>
#include <string>

#include <monome.h>

#include "../sequencer/part.hpp"
#include "../config/config.hpp"


class Instrument {
public:
  Instrument(Config *config) : config(config) {
    current_part_idx = 0;
    // TODO initialize better
    parts.push_back(new Part(0, config));
  };
  
  Part *get_current_part() {
    return parts[current_part_idx];
  };

  bool is_playing;
  bool stop_on_next_measure;
  
protected:
  std::string name;
  Config *config;

  int current_part_idx;
  std::vector<Part*> parts;
};

#endif
