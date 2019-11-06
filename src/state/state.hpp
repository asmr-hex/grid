#ifndef STATE_H
#define STATE_H

#include <map>
#include <string>

#include "../sequencer/types.hpp"
#include "../instruments/er1.hpp"
#include "../instruments/gr1.hpp"
#include "../instruments/ms20.hpp"
#include "../instruments/sp404.hpp"
#include "../instruments/instrument.hpp"



class State {
public:
  State(std::map<std::string, Instrument *> instrument_map) {
    initialize_instruments(instrument_map);
  };
  
  struct {
    bool shift_enabled;
    Microseconds step_period;
  } sequencer;

  struct {
    SP404 *sp404;
    MS20  *ms20;
    ER1   *er1;
    GR1   *gr1;
  } instruments;

private:
  void initialize_instruments(std::map<std::string, Instrument *> instrument_map) {
    for (auto it : instrument_map) {
      std::string name = it.first;
      if (name == SP404::name()) {
        instruments.sp404 = (SP404 *)(it.second);
      } else if (name == MS20::name()) {
        instruments.ms20 = (MS20 *)(it.second);
      } else if (name == ER1::name()) {
        instruments.er1 = (ER1 *)(it.second);
      } else if (name == GR1::name()) {
        instruments.gr1 = (GR1 *)(it.second);
      }
    }
  };
};

#endif
