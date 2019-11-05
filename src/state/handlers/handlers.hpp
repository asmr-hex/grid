#ifndef HANDLERS_H
#define HANDLERS_H

#include "handler_fn.hpp"


class EventHandlers {
public:
  EventHandlers() {
    // instantiate all handlers.
  };

private:
  std::vector<midi_handler> midi_handlers;
  std::vector<monome_handler> monome_handlers;
};

#endif
