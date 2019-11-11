#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <map>
#include <vector>
#include <string>

#include <monome.h>

#include "../sequencer/part.hpp"
#include "../config/config.hpp"
#include "../io/io.hpp"


class Instrument {
public:
  Instrument(Config *config, IO *io) : config(config), io(io) {
    current_part = 0;
    current_bank = 0;
    // TODO initialize better
    parts.push_back(new Part(0, config, io));
  };
  
  Part *get_current_part() {
    return parts[current_part];
  };

  int current_part;
  int current_bank;
  bool is_playing;
  bool stop_on_next_measure;
  
protected:
  std::string name;
  Config *config;
  IO *io;

  std::vector<Part*> parts;
};

#endif
