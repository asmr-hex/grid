#ifndef CONTROLLER_SHIFT_HANDLER_H
#define CONTROLLER_SHIFT_HANDLER_H

#include <monome.h>

#include "../../io/io.hpp"
#include "../../state/state.hpp"
#include "../../config/config.hpp"
#include "../../config/mappings/types.hpp"
#include "../../sequencer/part.hpp"

// TODO these handlers should never touch the leds of the step sequencer directly,
// this should entirely be the responisbility of the current Part...
// Question. should each visual component in the sequencer get its own class which
// handles the ui changes?....
// for example, maybe we should rip out all the steps ui logic from the Part class and
// throw it into a Steps class to visually represent the steps ui


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
      // only apply this playback toggle to the currently rendered instrument!
      state->instruments_by_name[state->sequencer.rendered_instrument]->is_playing =
        !state->instruments_by_name[state->sequencer.rendered_instrument]->is_playing;

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
    // get part under edit
    Part *part_under_edit = state->get_rendered_instrument()->get_part_under_edit();

    // set the current ppqn for this part
    switch (config->mappings.ppqn.get_sequential_index_from_coordinates(event->grid.x, event->grid.y)) {
    case 0:
      part_under_edit->ppqn = constants::PPQN::One;
      break;
    case 1:
      part_under_edit->ppqn = constants::PPQN::Two;
      break;
    case 2:
      part_under_edit->ppqn = constants::PPQN::Four;
      break;
    case 3:
      part_under_edit->ppqn = constants::PPQN::Eight;
      break;
    case 4:
      part_under_edit->ppqn = constants::PPQN::Sixteen;
      break;
    case 5:
      part_under_edit->ppqn = constants::PPQN::ThirtyTwo;
      break;
    case 6:
      part_under_edit->ppqn = constants::PPQN::SixtyFour;
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

void step_handler(IO *io, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    config->mappings.steps.x.min <= event->grid.x && event->grid.x <= config->mappings.steps.x.max &&
    config->mappings.steps.y.min <= event->grid.y && event->grid.y <= config->mappings.steps.y.max;
  if (!relevant) return;

  switch (event->event_type) {
  case MONOME_BUTTON_UP:
    // get step index
    Part *part_under_edit = state->get_rendered_instrument()->get_part_under_edit();
    unsigned int relative_step = config->mappings.steps.get_sequential_index_from_coordinates(event->grid.x, event->grid.y);
    unsigned int absolute_step = part_under_edit->get_absolute_step(part_under_edit->page.under_edit, relative_step);

    if (state->sequencer.last_step_enabled) {
      // escape hatch if we are in 'last_step' mode.
      part_under_edit->update_last_step({event->grid.x, event->grid.y});
      return;
    }
    
    if (part_under_edit->is_step_on(absolute_step)) {
      // turn this step off!

      // remove stop from part
      part_under_edit->remove_step(relative_step);

      monome_led_off(io->output.monome, event->grid.x, event->grid.y);
    } else {
      // turn this step on!!

      // TODO add step to part
      part_under_edit->add_step(relative_step);
      
      monome_led_on(io->output.monome, event->grid.x, event->grid.y);
    }
    break;
  }
}

void last_step_handler(IO *io, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    event->grid.x == config->mappings.last_step.x &&
    event->grid.y == config->mappings.last_step.y;
  if (!relevant) return;

  // get last step of current part
  Part *part_under_edit = state->get_rendered_instrument()->get_part_under_edit();
  int last_step = part_under_edit->length - 1;
  mapping_coordinates_t last_step_coords = config->mappings.steps.get_coordinates_from_sequential_index(last_step);
  mapping_coordinates_t last_step_page_coords = config->mappings.pages.get_coordinates_from_sequential_index(part_under_edit->page.last_step);
  mapping_coordinates_t rendered_page_coords = config->mappings.pages.get_coordinates_from_sequential_index(part_under_edit->page.rendered);
  mapping_coordinates_t page_being_edited_coords = config->mappings.pages.get_coordinates_from_sequential_index(part_under_edit->page.under_edit);
  
  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:
    // turn on 'show last step' button
    monome_led_on(io->output.monome, event->grid.x, event->grid.y);

    // before we do anything, it shift is being held, this is toggling the 'follow cursor' mode
    if (state->sequencer.shift_enabled) {
      state->sequencer.follow_cursor = !state->sequencer.follow_cursor;
      part_under_edit->follow_cursor = state->sequencer.follow_cursor;

      return;
    }

    // set the state for 'show last step'
    part_under_edit->show_last_step = true;
    state->sequencer.last_step_enabled = true; // TODO do we need this in the sequencer state????
    
    // only update the pages UI if it is not already being rendered
    if (part_under_edit->page.last_step != part_under_edit->page.rendered) {
      monome_led_level_set(io->output.monome, page_being_edited_coords.x, page_being_edited_coords.y, 4);
      monome_led_on(io->output.monome, last_step_page_coords.x, last_step_page_coords.y);
    }

    // render the page of the last step
    part_under_edit->render_page(part_under_edit->page.last_step);
    
    break;
  case MONOME_BUTTON_UP:
    // turn off 'last step' button
    monome_led_off(io->output.monome, event->grid.x, event->grid.y);

    // before we do anything, it shift is being held, this is toggling the 'follow cursor' mode
    // and unpressing the 'last step' button should be a noop.
    if (state->sequencer.shift_enabled) {
      return;
    }
    
    // set the state
    // TODO do we need both these states duplicated...?
    state->sequencer.last_step_enabled = false;
    part_under_edit->show_last_step = false;

    // only update the pages UI if it is not already being rendered
    if (part_under_edit->page.under_edit != part_under_edit->page.rendered) {
      monome_led_level_set(io->output.monome, rendered_page_coords.x, rendered_page_coords.y, 4);
      // monome_led_level_set(io->output.monome, last_step_tmp_page_coords.x, last_step_tmp_page_coords.y, 4); // note, i think we need to do this before the next line...
      monome_led_on(io->output.monome, page_being_edited_coords.x, page_being_edited_coords.y);
    }

    // re-render the page currently being edited
    part_under_edit->render_page(part_under_edit->page.under_edit);

    break;
  }
};

void page_select_handler(IO *io, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    config->mappings.pages.x.min <= event->grid.x && event->grid.x <= config->mappings.pages.x.max &&
    config->mappings.pages.y.min <= event->grid.y && event->grid.y <= config->mappings.pages.y.max;
  if (!relevant) return;

  // get current part & current page
  Part *part_under_edit = state->get_rendered_instrument()->get_part_under_edit();
  int rendered_page = part_under_edit->page.rendered;
  mapping_coordinates_t rendered_page_coords = config->mappings.pages.get_coordinates_from_sequential_index(rendered_page);

  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:
    if (!state->sequencer.follow_cursor || part_under_edit->show_last_step) {
      int new_page = config->mappings.pages.get_sequential_index_from_coordinates(event->grid.x, event->grid.y);

      // only render the new page if we are not following the cursor, or we are setting the page of the last step
      part_under_edit->render_page(new_page);
      monome_led_level_set(io->output.monome, rendered_page_coords.x, rendered_page_coords.y, 4);
      monome_led_on(io->output.monome, event->grid.x, event->grid.y);

      // if we are not setting the 'last step' page, set the new page to be the page under edit
      if (!part_under_edit->show_last_step) part_under_edit->page.under_edit = new_page;  
    }
    
    break;
  }
}

void part_select_handler(IO *io, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    config->mappings.parts.x.min <= event->grid.x && event->grid.x <= config->mappings.parts.x.max &&
    config->mappings.parts.y.min <= event->grid.y && event->grid.y <= config->mappings.parts.y.max;
  if (!relevant) return;

  Instrument *rendered_instrument = state->get_rendered_instrument();
  int selected_part_idx = config->mappings.parts.get_sequential_index_from_coordinates(event->grid.x, event->grid.y);
  
  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:

    if (state->sequencer.shift_enabled) {
      // immediately begin playing selected part.
      rendered_instrument->play_part(selected_part_idx);
      return;
    }

    // set the part under edit
    rendered_instrument->edit_part(selected_part_idx);
    break;
  }
}
#endif
