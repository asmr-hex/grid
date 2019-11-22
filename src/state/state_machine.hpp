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
#include "../animation/animator.hpp"
#include "../sequencer/constants.hpp"
#include "../instruments/instrument.hpp"


class StateMachine {
public:
  StateMachine(IO *io, Config *config, std::map<std::string, Instrument *> instruments, Animator *animation){
    state = new State(instruments);

    event_handlers = new EventHandlers(io, state, config, animation);

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

    // render instrument
    rendered_instrument->render();
  };
};

#endif
