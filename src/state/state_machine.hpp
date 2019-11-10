#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <map>
#include <string>

#include <boost/thread.hpp>

#include "state.hpp"
#include "../io/io.hpp"
#include "../config/config.hpp"
#include "../handlers/handlers.hpp"
#include "../sequencer/constants.hpp"
#include "../instruments/instrument.hpp"


class StateMachine {
public:
  StateMachine(IO *io, Config *config, std::map<std::string, Instrument *> instruments){
    state = new State(instruments);

    event_handlers = new EventHandlers(io, state, config);

    initialize_ui(io, state, config);
  };
  
  void listen() {
    boost::thread(&EventHandlers::register_monome_callback, event_handlers);
    boost::thread(&EventHandlers::register_midi_callback, event_handlers);
  };

  State *state;
  
private:
  EventHandlers *event_handlers;

  void initialize_ui(IO *io, State *state, Config *config) {
    // turn all leds off at first.
    monome_led_all(io->output.monome, 0);

    // set current instrument part ppqn led
    unsigned int ppqn_led_offset = 0;
    switch (state->get_current_part()->ppqn) {
    case constants::PPQN::One:
      ppqn_led_offset = 0;
      break;
    case constants::PPQN::Two:
      ppqn_led_offset = 1;
      break;
    case constants::PPQN::Four:
      ppqn_led_offset = 2;
      break;
    case constants::PPQN::Eight:
      ppqn_led_offset = 3;
      break;
    case constants::PPQN::Sixteen:
      ppqn_led_offset = 4;
      break;
    case constants::PPQN::ThirtyTwo:
      ppqn_led_offset = 5;
      break;
    case constants::PPQN::SixtyFour:
      ppqn_led_offset = 6;
      break;
    }
    mapping_coordinates_t selected_ppqn = config->mappings.ppqn.get_coordinates_from_sequential_index(ppqn_led_offset);
    monome_led_on(io->output.monome, selected_ppqn.x, selected_ppqn.y);

    // TODO set selected sequence led

    // TODO set selected bank led
  };
};

#endif
