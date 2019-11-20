#ifndef PART_H
#define PART_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "utils.hpp"
#include "types.hpp"
#include "sequence.hpp"
#include "constants.hpp"

#include "../io/io.hpp"

#include "../config/config.hpp"
#include "../config/mappings/types.hpp"
#include "../handlers/utils.hpp"

// TODO feed io to Part so it can render its internal state!

class Part {
public:
  int id;
  int ppqn = 8;
  struct {
    int rendered = 0;
    int under_edit = 0;
    int last_step = 1; // refactor to of_last_step
    int cursor = 0;
  } page;
  bool follow_cursor = false;
  int length = 32; // TODO refactor this to be last_step
  bool show_last_step = false;
  struct {
    bool is_playing = false;
  } playback;
  bool unsaved;

  
  Part(int id, Config *config, IO *io) : id(id), config(config), io(io) {
    // load part file if it exists.
    ppqn = 8;
    length = 64;
    default_note = "C5";
  };

  void stop_playback() {
    playback.is_playing = false;
    active_step = 0;
  };
  
  void advance_ui_cursor() {
    int coarse_step = active_step / constants::PPQN_MAX;
    bool current_step_is_visible = is_step_visible(coarse_step);

    if (current_step_is_visible && !is_step_on(coarse_step)) {
      int page_relative_coarse_step = get_relative_step(page.rendered, coarse_step);
      mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(page_relative_coarse_step);
      
      // we want to turn this step OFF on the next advance.
      monome_led_off(io->output.monome, coords.x, coords.y);
    }

    // now lets look at the next step
    int next_coarse_step = get_next_step(active_step) / constants::PPQN_MAX;
    bool next_step_is_visible = is_step_visible(next_coarse_step);

    if (next_step_is_visible) {
      int page_relative_coarse_step = get_relative_step(page.rendered, next_coarse_step);
      mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(page_relative_coarse_step);

      // we want to turn this step ON on the next advance.
      monome_led_on(io->output.monome, coords.x, coords.y);
    }

    // update the cursor's page if necessary
    int page_of_next_step = get_page(next_coarse_step);
    if (page_of_next_step != page.cursor) {
      page.cursor = get_page(next_coarse_step);

      // render the next page if necessary
      if (follow_cursor) {
      
        // render cursor page only if we aren't showing the last step
        if (!show_last_step) render_page(page.cursor);

        // set page under edit as cursor page
        page.under_edit = page.cursor;
      }
      
      // update the page UI
      render_page_selection_ui();
    }
  };
  
  std::vector<step_event_t> advance() {
    std::vector<step_event_t> next_events;

    // TODO if playback is not playing, do not advance.

    // get all next events
    sequence.collect_all_events_at(active_step, &next_events);

    // advance to next step
    active_step = get_next_step(active_step);
    
    return next_events;
  };
  
  // add a step to the sequence. this adds the default midi note to the sequence.
  void add_step(unsigned int coarse_step_idx) {
    int abs_coarse_step = get_absolute_step(page.under_edit, coarse_step_idx);
    step_event_t event = midi_note_on(default_note, 0, 127);
    step_idx_t step = get_fine_step_index(abs_coarse_step);

    // TODO ....we should probably be using a mutex... -___-
    sequence.add_midi_note_on_event(event, step, 1);

    // add step to rendered steps
    rendered_steps[page.under_edit].insert(coarse_step_idx);
  };
  
  void remove_step(unsigned int coarse_step_idx) {
    int abs_coarse_step = get_absolute_step(page.under_edit, coarse_step_idx);
    step_idx_t step = get_fine_step_index(abs_coarse_step);
    sequence.remove_step(step);

    // remove step from rendered steps
    rendered_steps[page.under_edit].erase(coarse_step_idx);
  };

  // sets the ppqn given a sequential index.
  void set_ppqn(int ppqn_index) {
    switch (ppqn_index) {
    case 0:
      ppqn = constants::PPQN::One;
      break;
    case 1:
      ppqn = constants::PPQN::Two;
      break;
    case 2:
      ppqn = constants::PPQN::Four;
      break;
    case 3:
      ppqn = constants::PPQN::Eight;
      break;
    case 4:
      ppqn = constants::PPQN::Sixteen;
      break;
    case 5:
      ppqn = constants::PPQN::ThirtyTwo;
      break;
    case 6:
      ppqn = constants::PPQN::SixtyFour;
      break;
    }
  };
  
  // renders a page onto the ui without forced re-rendering
  void render_page(int new_page) {
    render_page(new_page, false);
  };

  // renders a page onto the ui
  void render_page(int new_page, bool force_rerender) {
    if (page.rendered != new_page || force_rerender) {
      page.rendered = new_page;

      // render all visibile steps on this page
      set_led_region_intensity(io, &config->mappings.steps, 0);
      for (auto i : rendered_steps[page.rendered]) {
        mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(i);
        monome_led_on(io->output.monome, coords.x, coords.y);
      }      
    }

    // if we are rendering the pafge with the last step, show or hide last step
    if (page.last_step == new_page) {
      int relative_last_step = get_relative_step(new_page, length -1);
      mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(relative_last_step);
      
      if (show_last_step) {
        // show the last step
        monome_led_on(io->output.monome, coords.x, coords.y);  
      } else if (!show_last_step && !is_step_on(get_last_step())) {
        // hide the last step unless that step is on
        monome_led_off(io->output.monome, coords.x, coords.y);
      }
    }
  };

  // renders the page selection panel in the ui (monome grid).
  //
  // there should typically be two pages highlighted at a given time:
  //  1) the currently rendered page
  //  2) the page the cursor is on
  // when these pages are the same, there will only be one page highlighted.
  void render_page_selection_ui() {
    // set all pages to blank
    set_led_region_intensity(io, &config->mappings.pages, 4);

    // set rendered page to highlighted
    mapping_coordinates_t rendered_page_coords = config->mappings.pages.get_coordinates_from_sequential_index(page.rendered);
    monome_led_on(io->output.monome, rendered_page_coords.x, rendered_page_coords.y);

    if (page.rendered != page.cursor) {
      // set cursor page to highlighted
      mapping_coordinates_t cursor_page_coords = config->mappings.pages.get_coordinates_from_sequential_index(page.cursor);
      monome_led_on(io->output.monome, cursor_page_coords.x, cursor_page_coords.y); 
    }
  };

  // renders the ppqn selection panel in the ui (monome grid).
  void render_ppqn_selection_ui() {
    int ppqn_index = get_ppqn_index(ppqn);
    mapping_coordinates_t selected_ppqn = config->mappings.ppqn.get_coordinates_from_sequential_index(ppqn_index);

    // turn off all leds in ppqn zone...
    // It might be more efficient to only turn off the previous ppqn button pressed, but we would
    // need to keep track of that in teh state... might be worthwhile in the future depending on
    // what other regions need in terms of functionality.
    for (unsigned int x = config->mappings.ppqn.x.min; x <= config->mappings.ppqn.x.max; x++) {
      for (unsigned int y = config->mappings.ppqn.y.min; y <= config->mappings.ppqn.y.max; y++) {
        monome_led_off(io->output.monome, x, y);
      }      
    }
    
    monome_led_on(io->output.monome, selected_ppqn.x, selected_ppqn.y);
  };
  
  // updates the state of last step of a part and updates the ui (monome grid).
  //
  // this function assumes that the currently rendered page is the page on which
  // the new last step lies. this is important because the coordinates supplied for
  // the new last step are relative to the currently rendered page.
  void update_last_step(mapping_coordinates_t grid) {
    int new_last_step = get_absolute_step(page.rendered, config->mappings.steps.get_sequential_index_from_coordinates(grid.x, grid.y));

    // turn off led for outdated last step if it was on the same page as the new last step
    // but only if the outdated last step was not already on as an active step.
    if (page.last_step == page.rendered && !is_step_on(get_last_step())) {
      // TODO if there is a step activated on the last step, we need to differentiate...

      mapping_coordinates_t old_last_step = config->mappings.steps.get_coordinates_from_sequential_index(get_relative_step(page.rendered, get_last_step()));
      monome_led_off(io->output.monome, old_last_step.x, old_last_step.y);
    }

    // update the state for the last step (length)
    length = new_last_step + 1;

    // update the page on which the last step lies. again, we assume this is the currently
    // rendered page.
    page.last_step = page.rendered;

    // turn on the led for the new last step.
    monome_led_on(io->output.monome, grid.x, grid.y);
  };

  bool is_step_on(unsigned int coarse_step_idx) {
    step_idx_t step = get_fine_step_index(coarse_step_idx);
    return sequence.is_step_on(step, active_layer);
  };

  int get_last_step() {
    return length - 1;
  };

  int get_page(int absolute_coarse_step) {
    unsigned int page_size = config->mappings.steps.get_area();
    return absolute_coarse_step / page_size;
  };
  
  int get_page_relative_last_step(int page_i) {
    return get_relative_step(page_i, get_last_step());
  };

  int get_absolute_step(int page_i, int page_relative_step) {
    return (config->mappings.steps.get_area() * page_i) + page_relative_step;
  };

  int get_relative_step(int page_i, int page_absolute_step) {
    return page_absolute_step - (config->mappings.steps.get_area() * page_i);
  };
    
private:
  int active_step = 0;
  event_uid_t active_layer = 0x0000;  // 0x0000 is the 'all' layer
  Sequence sequence;
  std::string default_note;
  Config *config;
  IO *io;
  std::map<int, std::set<int> > rendered_steps;

  int get_next_step(int step) {
    // advance to next step
    step += ppqn;

    // if the active step is now greater than the last step, loop back
    if (step > (length * constants::PPQN_MAX) - 1) {
      step = 0;
    }

    return step;
  };

  // returns the sequential index of ppqn within the available ppqns sorted in
  // ascending order.
  int get_ppqn_index(int ppqn_i) {
    int index;
    
    switch (ppqn_i) {
    case constants::PPQN::One:
      index = 0;
      break;
    case constants::PPQN::Two:
      index = 1;
      break;
    case constants::PPQN::Four:
      index = 2;
      break;
    case constants::PPQN::Eight:
      index = 3;
      break;
    case constants::PPQN::Sixteen:
      index = 4;
      break;
    case constants::PPQN::ThirtyTwo:
      index = 5;
      break;
    case constants::PPQN::SixtyFour:
      index = 6;
      break;
    default:
      index = 0;
      break;
    }

    return index;
  };

  bool is_step_visible(int coarse_step) {
    unsigned int page_size = config->mappings.steps.get_area();
    int min_visible_step = (page.rendered * page_size);
    int max_visible_step = ((page.rendered + 1) * page_size) - 1;
    return min_visible_step <= coarse_step && coarse_step <= max_visible_step;
  };
};


#endif
