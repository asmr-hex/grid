#ifndef HANDLERS_H
#define HANDLERS_H

#include <vector>

#include <monome.h>

#include "handler_fn.hpp"


class EventHandlers {
public:
  EventHandlers(IO * io, State *state) : io(io), state(state) {
    // TODO instantiate all handlers.
  };

  void register_midi_callback() {
    io->input.midi->setCallback(this->handle_midi_in, this);
  };

  void register_monome_callback() {
    monome_register_handler(io->input.monome, MONOME_BUTTON_DOWN, this->handle_monome_in, this);
    monome_register_handler(io->input.monome, MONOME_BUTTON_UP, this->handle_monome_in, this);

    monome_event_loop(io->input.monome);
  };
  
  static void handle_midi_in(double deltatime, std::vector< unsigned char > *message, void *userData) {
    EventHandlers * handlers = (EventHandlers *)userData;

    for (midi_handler fn : handlers->midi_handlers) {
      fn(handlers->io, handlers->state, message);
    }
  };

  static void handle_monome_in(const monome_event_t *event, void *userData) {
    EventHandlers * handlers = (EventHandlers *)userData;
    
    for (monome_handler fn : handlers->monome_handlers) {
      fn(handlers->io, handlers->state, event);
    }
  };
  
private:
  IO *io;
  State *state;

  std::vector<midi_handler> midi_handlers;
  std::vector<monome_handler> monome_handlers;
};

#endif
