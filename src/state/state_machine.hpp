#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <map>
#include <string>

#include <boost/thread.hpp>

#include "state.hpp"
#include "../io/io.hpp"
#include "../config/config.hpp"
#include "../handlers/utils.hpp"
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

    // TODO delegate this to ui initialization within instruments...
    
    // get the current instrument and part
    Instrument *rendered_instrument = state->get_rendered_instrument();
    Part *rendered_part = rendered_instrument->get_part_under_edit();
    
    // set current instrument part ppqn led
    set_led_region_intensity(io, &config->mappings.ppqn, 0);
    unsigned int ppqn_led_offset = 0;
    switch (rendered_part->ppqn) {
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

    // set selected sequence led
    set_led_region_intensity(io, &(config->mappings.parts), 0);
    mapping_coordinates_t selected_part = config->mappings.parts.get_coordinates_from_sequential_index(rendered_instrument->part.under_edit);
    monome_led_on(io->output.monome, selected_part.x, selected_part.y);

    // set selected bank led
    set_led_region_intensity(io, &(config->mappings.banks), 4);
    mapping_coordinates_t selected_bank = config->mappings.banks.get_coordinates_from_sequential_index(rendered_instrument->bank.under_edit);
    monome_led_on(io->output.monome, selected_bank.x, selected_bank.y);

    // set pages led
    set_led_region_intensity(io, &(config->mappings.pages), 4);
    mapping_coordinates_t selected_page = config->mappings.pages.get_coordinates_from_sequential_index(rendered_part->page.under_edit);
    monome_led_on(io->output.monome, selected_page.x, selected_page.y);
  };
};

#endif
