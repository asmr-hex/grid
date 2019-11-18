#ifndef STATE_H
#define STATE_H

#include <map>
#include <string>

#include "../sequencer/part.hpp"
#include "../sequencer/types.hpp"
#include "../sequencer/constants.hpp"

#include "../config/er1.hpp"
#include "../config/gr1.hpp"
#include "../config/ms20.hpp"
#include "../config/sp404.hpp"

#include "../instruments/er1.hpp"
#include "../instruments/gr1.hpp"
#include "../instruments/ms20.hpp"
#include "../instruments/sp404.hpp"
#include "../instruments/instrument.hpp"



class State {
public:
  State(std::map<std::string, Instrument *> instrument_map) : instruments_by_name(instrument_map) {
    initialize_instruments();

    // TODO initialize state
    sequencer.active_instrument = "sp404";
    float bpm = 120.0;
    sequencer.step_period = Microseconds(static_cast<int>((60 * 1000 * 1000)/(bpm * (float)constants::PPQN_MAX)));
  };
  
  struct {
    bool shift_enabled = false;
    bool follow_cursor = false;
    bool last_step_enabled = false; // TODO do we need this????
    Microseconds step_period;
    std::string active_instrument;
    struct {
      bool is_playing;
    } playback;
  } sequencer;

  struct {
    SP404 *sp404;
    MS20  *ms20;
    ER1   *er1;
    GR1   *gr1;
  } instruments;

  std::map<std::string, Instrument *> instruments_by_name;

  Instrument *get_current_instrument() {
    return instruments_by_name[sequencer.active_instrument];
  };

  Part *get_current_part() {
    return get_current_instrument()->get_current_part();
  };
  
private:
  void initialize_instruments() {
    for (auto it : instruments_by_name) {
      std::string name = it.first;
      if (name == SP404Config::name()) {
        instruments.sp404 = (SP404 *)(it.second);
      } else if (name == MS20Config::name()) {
        instruments.ms20 = (MS20 *)(it.second);
      } else if (name == ER1Config::name()) {
        instruments.er1 = (ER1 *)(it.second);
      } else if (name == GR1Config::name()) {
        instruments.gr1 = (GR1 *)(it.second);
      }
    }
  };
};

#endif
