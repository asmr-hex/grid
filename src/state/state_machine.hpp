#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "handlers/handlers.hpp"


class StateMachine {
public:
  void listen();
private:
  IO *io;
  State *state;
  EventHandlers *event_handlers;

  void handle_midi_in();
  void handle_monome_in();
};

#endif
