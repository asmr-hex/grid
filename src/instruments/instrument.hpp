#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <map>
#include <vector>
#include <string>

#include <monome.h>

#include "../sequencer/part.hpp"


class Instrument {
public:
  Instrument() {
    current_part_idx = 0;
    // TODO initialize better
    parts.push_back(new Part(0));
  };
  
  Part *get_current_part() {
    return parts[current_part_idx];
  };
  
protected:
  std::string name;

  int current_part_idx;
  std::vector<Part*> parts;
  
};

#endif
