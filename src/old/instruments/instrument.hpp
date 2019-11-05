#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <map>
#include <vector>
#include <string>

#include <monome.h>

#include "part.hpp"
#include "../consts.hpp"


// forward declaration to avoid circular dependency
class State;

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
  
  virtual void update_state(monome_event_t *) = 0;
private:
  std::string id;

  State *state;

  int current_part_idx;
  std::vector<Part*> parts;
  
};

#endif
