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

    // render default part under edit.
    rendered_instrument->render_part();
    // TODO eventually, once we are rendering the actual instrument UI, we should
    // have a method on Instrument called "render" AND a method on Part called "render"

    // // set selected sequence led
    // set_led_region_intensity(io, &(config->mappings.parts), 0);
    // mapping_coordinates_t selected_part = config->mappings.parts.get_coordinates_from_sequential_index(rendered_instrument->part.under_edit);
    // monome_led_on(io->output.monome, selected_part.x, selected_part.y);

    // // set selected bank led
    // set_led_region_intensity(io, &(config->mappings.banks), 4);
    // mapping_coordinates_t selected_bank = config->mappings.banks.get_coordinates_from_sequential_index(rendered_instrument->bank.under_edit);
    // monome_led_on(io->output.monome, selected_bank.x, selected_bank.y);

    // // set pages led
    // set_led_region_intensity(io, &(config->mappings.pages), 4);
    // mapping_coordinates_t selected_page = config->mappings.pages.get_coordinates_from_sequential_index(rendered_part->page.under_edit);
    // monome_led_on(io->output.monome, selected_page.x, selected_page.y);
  };
};

#endif
