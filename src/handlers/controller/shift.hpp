#ifndef CONTROLLER_SHIFT_HANDLER_H
#define CONTROLLER_SHIFT_HANDLER_H

#include <monome.h>

#include "../../io/io.hpp"
#include "../../state/state.hpp"
#include "../../config/config.hpp"
#include "../../config/mappings/types.hpp"
#include "../../sequencer/part.hpp"
#include "../../animation/types.hpp"
#include "../../animation/animator.hpp"


void shift_handler(IO *io, Animator *animation, State *state, Config *config, const monome_event_t *event) {
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

void play_pause_handler(IO *io, Animator *animation, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    event->grid.x == config->mappings.play_pause.x &&
    event->grid.y == config->mappings.play_pause.y &&
    !state->sequencer.transport.stop_is_held;
  if (!relevant) return;

  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:
    state->sequencer.transport.play_pause_is_held = true;

    // TODO toggle play/pause led to be the opposite state of the part in playback
    break;
  case MONOME_BUTTON_UP:
    state->sequencer.transport.play_pause_is_held = false;

    if (state->sequencer.transport.instruments.were_selected) {
      // while the play/pause button was held down, some instruments were selected!

      // TODO handle play or pausing all selected instruments.
      // TODO this would be a good time to refactr the 'sequence' field of 'State'
      // into its own separate class so it can have methods....liek the one to implement this.

      // reset transport instruments selection trigger
      state->sequencer.transport.instruments.were_selected = false;
      // TODO clear vector (or whatever data structure we use here...)
      
      return;
    }

    // just play or pause the currently under edit part for the rendered instrument.
    state->get_rendered_instrument()->play_or_pause_part();
    
    break;
  }
};

void stop_handler(IO *io, Animator *animation, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    event->grid.x == config->mappings.stop.x &&
    event->grid.y == config->mappings.stop.y &&
    !state->sequencer.transport.play_pause_is_held;
  if (!relevant) return;

  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:
    state->sequencer.transport.stop_is_held = true;
    monome_led_on(io->output.monome, event->grid.x, event->grid.y);
    break;
  case MONOME_BUTTON_UP:
    state->sequencer.transport.stop_is_held = false;
    monome_led_off(io->output.monome, event->grid.x, event->grid.y);

    if (state->sequencer.transport.instruments.were_selected) {
      // TODO handle stopping all selected instruments.
      // TODO this would be a good time to refactr the 'sequence' field of 'State'
      // into its own separate class so it can have methods....liek the one to implement this.

      // reset transport instruments selection trigger
      state->sequencer.transport.instruments.were_selected = false;
      // TODO clear vector (or whatever data structure we use here...)

      return;
    }

    // okay nothing was selected, just stop the part in playback
    state->get_rendered_instrument()->stop_part();
      
    break;
  }
};

void ppqn_handler(IO *io, Animator *animation, State *state, Config *config, const monome_event_t *event) {
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
    part_under_edit->set_ppqn(config->mappings.ppqn.get_sequential_index_from_coordinates(event->grid.x, event->grid.y));
    part_under_edit->render_ppqn_selection_ui();
  }
}

void step_handler(IO *io, Animator *animation, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    config->mappings.steps.x.min <= event->grid.x && event->grid.x <= config->mappings.steps.x.max &&
    config->mappings.steps.y.min <= event->grid.y && event->grid.y <= config->mappings.steps.y.max;
  if (!relevant) return;

  // TODO refactor all this to be encapsulated within Part (?)
  
  switch (event->event_type) {
  case MONOME_BUTTON_UP:
    // get step index
    Part *part_under_edit = state->get_rendered_instrument()->get_part_under_edit();
    unsigned int relative_step = config->mappings.steps.get_sequential_index_from_coordinates(event->grid.x, event->grid.y);
    unsigned int absolute_step = part_under_edit->get_absolute_step(part_under_edit->page.under_edit, relative_step);

    if (part_under_edit->show_last_step) {
      // escape hatch if we are in 'last_step' mode.
      part_under_edit->update_last_step({event->grid.x, event->grid.y});
      return;
    }
    
    if (part_under_edit->is_step_on(absolute_step)) {
      // turn this step off! remove stop from part
      part_under_edit->remove_step(relative_step);

      monome_led_off(io->output.monome, event->grid.x, event->grid.y);
    } else {
      // turn this step on!! add step to part
      part_under_edit->add_step(relative_step);
      
      monome_led_on(io->output.monome, event->grid.x, event->grid.y);
    }
    break;
  }
}

void last_step_handler(IO *io, Animator *animation, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    event->grid.x == config->mappings.last_step.x &&
    event->grid.y == config->mappings.last_step.y;
  if (!relevant) return;

  // get last step of current part
  Part *part_under_edit = state->get_rendered_instrument()->get_part_under_edit();
  int last_step = part_under_edit->length - 1;

  // TODO refactor all this into encapsulated method in Part (?)
  
  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:

    // before we do anything, it shift is being held, this is toggling the 'follow cursor' mode
    if (state->sequencer.shift_enabled) {
      part_under_edit->follow_cursor = !part_under_edit->follow_cursor;
      part_under_edit->page.under_edit = part_under_edit->page.cursor;
    }

    // set the state for 'show last step'
    part_under_edit->show_last_step = true;

    part_under_edit->render_last_step_ui();
    
    // render the page of the last step
    part_under_edit->render_page(part_under_edit->page.last_step);

    // re-render page selection ui
    part_under_edit->render_page_selection_ui();
    
    break;
  case MONOME_BUTTON_UP:
    // turn off 'last step' button
    monome_led_off(io->output.monome, event->grid.x, event->grid.y);
    
    // set the state
    part_under_edit->show_last_step = false;

    // re-render the page currently being edited
    part_under_edit->render_page(part_under_edit->page.under_edit);

    // re-render page selection ui
    part_under_edit->render_page_selection_ui();
    
    break;
  }
};

void page_select_handler(IO *io, Animator *animation, State *state, Config *config, const monome_event_t *event) {
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
    // TODO refactor all this into a method encapsulated in Part
    
    // if we are following the cursor and then select a new page, turn off follow mode
    // (unless we are selecting a new last step)
    if (part_under_edit->follow_cursor && !part_under_edit->show_last_step) {
      part_under_edit->follow_cursor = false;
      animation->remove(config->mappings.last_step);
      monome_led_off(io->output.monome, config->mappings.last_step.x, config->mappings.last_step.y);
    }
    
    int new_page = config->mappings.pages.get_sequential_index_from_coordinates(event->grid.x, event->grid.y);

    // only render the new page if we are not following the cursor, or we are setting the page of the last step
    part_under_edit->render_page(new_page);

    // if we are not setting the 'last step' page, set the new page to be the page under edit
    if (!part_under_edit->show_last_step) {
      part_under_edit->page.under_edit = new_page;
      part_under_edit->render_page_selection_ui();
    } else {
      part_under_edit->render_page_selection_ui(new_page);
    }
    
    break;
  }
};

void part_select_handler(IO *io, Animator *animation, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    config->mappings.parts.x.min <= event->grid.x && event->grid.x <= config->mappings.parts.x.max &&
    config->mappings.parts.y.min <= event->grid.y && event->grid.y <= config->mappings.parts.y.max;
  if (!relevant) return;

  Instrument *rendered_instrument = state->get_rendered_instrument();
  int selected_part_idx = config->mappings.parts.get_sequential_index_from_coordinates(event->grid.x, event->grid.y);
  
  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:
    // set the part under edit
    rendered_instrument->edit_part(selected_part_idx);
    break;
  }
};

void bank_select_handler(IO *io, Animator *animation, State *state, Config *config, const monome_event_t *event) {
  // is this event even relevant?
  bool relevant =
    config->mappings.banks.x.min <= event->grid.x && event->grid.x <= config->mappings.banks.x.max &&
    config->mappings.banks.y.min <= event->grid.y && event->grid.y <= config->mappings.banks.y.max;
  if (!relevant) return;

  Instrument *rendered_instrument = state->get_rendered_instrument();
  int selected_bank_idx = config->mappings.banks.get_sequential_index_from_coordinates(event->grid.x, event->grid.y);
  
  switch (event->event_type) {
  case MONOME_BUTTON_DOWN:
    // set the part/bank under edit
    rendered_instrument->edit_part(selected_bank_idx, rendered_instrument->part.under_edit);
  }
};

#endif
