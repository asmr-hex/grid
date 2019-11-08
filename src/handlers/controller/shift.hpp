#ifndef CONTROLLER_SHIFT_HANDLER_H
#define CONTROLLER_SHIFT_HANDLER_H

#include <monome.h>

#include "../../io/io.hpp"
#include "../../state/state.hpp"
#include "../../config/config.hpp"


void shift_handler(IO *io, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  if (event->grid.x != config->mappings.shift.x && event->grid.y != config->mappings.shift.y) return;
  
  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:
    // turn shift on
    state->shift_enabled = true;
    monome_led_on(io->output.monome, event->grid.x, event->grid.y);
    break;
  case MONOME_BUTTON_UP:
    // turn shift off
    state->shift_enabled = false;
    monome_led_off(io->output.monome, event->grid.x, event->grid.y);
    break;
  }
};

void play_stop_handler(IO *io, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  if (event->grid.x != config->mappings.play_stop.x && event->grid.y != config->mappings.play_stop.y) return;

  switch (event->event_type) {
  case MONOME_BUTTON_UP:
    // toggle playback

    if (state->sequencer.shift_enabled) {
      // only apply this playback toggle to the currently active instrument!
      state->instruments_by_name[state->sequencer.active_instrument]->is_playing =
        !state->instruments_by_name[state->sequencer.active_instrument]->is_playing;

      return;
    }
    break;
  }
}

#endif
