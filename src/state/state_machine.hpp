#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <map>
#include <string>

#include <boost/thread.hpp>

#include "state.hpp"
#include "../io/io.hpp"
#include "../config/config.hpp"
#include "../handlers/handlers.hpp"
#include "../instruments/instrument.hpp"


class StateMachine {
public:
  StateMachine(IO *io, Config *config, std::map<std::string, Instrument *> instruments){
    state = new State(instruments);

    event_handlers = new EventHandlers(io, state, config);
  };
  
  void listen() {
    boost::thread(&EventHandlers::register_monome_callback, event_handlers);
    boost::thread(&EventHandlers::register_midi_callback, event_handlers);
  };

  State *state;
  
private:
  EventHandlers *event_handlers;
};

#endif
