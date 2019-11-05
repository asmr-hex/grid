#ifndef STATE_HANDLERS_H
#define STATE_HANDLERS_H

#include <monome.h>

#include "state.hpp"
#include "controllers/grid.hpp"


class ShiftButton : public Button {
public:
  ShiftButton(monome_t *monome, unsigned int x, unsigned int y) : Button(monome, x, y) {};
protected:
  void handle(monome_event_t *event, State *state) {
    switch (event->event_type) {
    case MONOME_BUTTON_DOWN:
      state->shift_enabled = true;
      monome_led_on(monome, event->grid.x, event->grid.y);
      break;
    case MONOME_BUTTON_UP:
      state->shift_enabled = false;
      monome_led_off(monome, event->grid.x, event->grid.y);
      break;
    }
  };
};


#endif
