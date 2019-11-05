#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <boost/thread.hpp>

#include "state.hpp"
#include "../io/io.hpp"
#include "../handlers/handlers.hpp"


class StateMachine {
public:
  StateMachine(IO *io, State *state) : io(io), state(state), event_handlers(io, state) {
    // TODO initialize state
  };
  
  void listen() {
    boost::thread(&EventHandlers::register_monome_callback, &event_handlers);
    boost::thread(&EventHandlers::register_midi_callback, &event_handlers);
  };
  
private:
  IO *io;
  State *state;
  EventHandlers event_handlers;
};

#endif
