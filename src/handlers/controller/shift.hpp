#ifndef CONTROLLER_SHIFT_HANDLER_H
#define CONTROLLER_SHIFT_HANDLER_H

#include <monome.h>

#include "../../io/io.hpp"
#include "../../state/state.hpp"
#include "../../config/config.hpp"
#include "../../sequencer/part.hpp"


void shift_handler(IO *io, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    event->grid.x == config->mappings.shift.x &&
    event->grid.y == config->mappings.shift.y;
  if (!relevant) return;
  
  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:
    // turn shift on
    state->sequencer.shift_enabled = true;
    monome_led_on(io->output.monome, event->grid.x, event->grid.y);
    break;
  case MONOME_BUTTON_UP:
    // turn shift off
    state->sequencer.shift_enabled = false;
    monome_led_off(io->output.monome, event->grid.x, event->grid.y);
    break;
  }
};

void play_stop_handler(IO *io, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    event->grid.x == config->mappings.play_stop.x &&
    event->grid.y == config->mappings.play_stop.y;
  if (!relevant) return;

  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:
    // toggle playback
    if (state->sequencer.shift_enabled) {
      // only apply this playback toggle to the currently active instrument!
      state->instruments_by_name[state->sequencer.active_instrument]->is_playing =
        !state->instruments_by_name[state->sequencer.active_instrument]->is_playing;

      // TODO (coco|11.9.2019) include logic to toggle "instrument local" playback leds on shift
      // AND here too
    } else {
      // apply to global playback state
      bool is_playing = state->sequencer.playback.is_playing;
      state->sequencer.playback.is_playing = !is_playing;

      if (is_playing) monome_led_off(io->output.monome, event->grid.x, event->grid.y);
      else monome_led_on(io->output.monome, event->grid.x, event->grid.y);

    }
    break;
  }
}

void ppqn_handler(IO *io, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    config->mappings.ppqn.x.min <= event->grid.x && event->grid.x <= config->mappings.ppqn.x.max &&
    config->mappings.ppqn.y.min <= event->grid.y && event->grid.y <= config->mappings.ppqn.y.max;
  if (!relevant) return;

  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:
    // get current part playing
    Part *current_part = state->instruments_by_name[state->sequencer.active_instrument]->get_current_part();

    // set the current ppqn for this part
    switch (config->mappings.ppqn.get_sequential_index_from_coordinates(event->grid.x, event->grid.y)) {
    case 0:
      current_part->ppqn = constants::PPQN::One;
      break;
    case 1:
      current_part->ppqn = constants::PPQN::Two;
      break;
    case 2:
      current_part->ppqn = constants::PPQN::Four;
      break;
    case 3:
      current_part->ppqn = constants::PPQN::Eight;
      break;
    case 4:
      current_part->ppqn = constants::PPQN::Sixteen;
      break;
    case 5:
      current_part->ppqn = constants::PPQN::ThirtyTwo;
      break;
    case 6:
      current_part->ppqn = constants::PPQN::SixtyFour;
      break;
    }
    

    // turn off all leds in ppqn zone...
    // It might be more efficient to only turn off the previous ppqn button pressed, but we would
    // need to keep track of that in teh state... might be worthwhile in the future depending on
    // what other regions need in terms of functionality.
    for (unsigned int x = config->mappings.ppqn.x.min; x <= config->mappings.ppqn.x.max; x++) {
      for (unsigned int y = config->mappings.ppqn.y.min; y <= config->mappings.ppqn.y.max; y++) {
        monome_led_off(io->output.monome, x, y);
      }      
    }

    // turn on new part ppqn led
    monome_led_on(io->output.monome, event->grid.x, event->grid.y);
  }
}

#endif
