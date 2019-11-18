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
  Part(int id, Config *config, IO *io) : id(id), config(config), io(io) {
    // load part file if it exists.
    ppqn = 8;
    length = 64;
    default_note = "C5";
    active_step = 0;
  };

  std::vector<step_event_t> advance(bool instrument_is_displayed) {
    std::vector<step_event_t> next_events;

    // should we display the step in the ui?
    collect_next_ui_step_events(instrument_is_displayed, &next_events);

    // TODO if playback is not playing, do not advance.

    // get all next events
    sequence.collect_all_events_at(active_step, &next_events);

    // advance to next step
    active_step = get_next_step(active_step);
    
    return next_events;
  };

  bool is_step_on(unsigned int coarse_step_idx) {
    step_idx_t step = get_fine_step_index(coarse_step_idx);
    return sequence.is_step_on(step, active_layer);
  };

  /*
    add a step to the sequence. this adds the default midi note to
    the sequence.
   */
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

  void render_page(int new_page) {
    if (page.rendered != new_page) {
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
    
    // // show the last step on this page if appropriate
    // if (show_last_step && page.last_step == new_page) {
    //   int relative_last_step = get_relative_step(new_page, length -1);
    //   mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(relative_last_step);
    //   monome_led_on(io->output.monome, coords.x, coords.y);
    // }
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
  }
  
  int get_last_step() {
    return length - 1;
  }

  int get_page_relative_last_step(int page_i) {
    return get_relative_step(page_i, get_last_step());
  }

  int get_absolute_step(int page_i, int page_relative_step) {
    return (config->mappings.steps.get_area() * page_i) + page_relative_step;
  }

  int get_relative_step(int page_i, int page_absolute_step) {
    return page_absolute_step - (config->mappings.steps.get_area() * page_i);
  }
  
  int id;
  int ppqn = 8;
  struct {
    int rendered = 0;
    int under_edit = 0;
    int last_step = 1; // refactor to of_last_step
    int tmp_last_step = 1; // refactor to of_last_step_tmp ??
  } page;
  int length = 32; // TODO refactor this to be last_step
  bool show_last_step = false;
  bool unsaved;
  
private:
  int active_step;
  event_uid_t active_layer = 0x0000;  // 0x0000 is the 'all' layer
  Sequence sequence;
  std::string default_note;
  Config *config;
  IO *io;
  std::map<int, std::set<int> > rendered_steps;

  int get_next_step(int step) {
    // advance to next step
    step += ppqn;

    // if the active step is now greater than the last step, circle back
    if (step > (length * constants::PPQN_MAX) - 1) {
      step = 0;
    }

    return step;
  };
  
  void collect_next_ui_step_events(bool instrument_is_displayed, std::vector<step_event_t> *collector) {
    // escape immediately if this is not the currently active isntrument.
    if (!instrument_is_displayed) return;

    int coarse_step = active_step / constants::PPQN_MAX;

    if (is_step_visible(coarse_step) && !is_step_on(coarse_step)) {
      int page_relative_coarse_step = get_relative_step(page.rendered, coarse_step);
      mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(page_relative_coarse_step);
      
      // we want to turn this step OFF on the next advance.
      collector->push_back(turn_led_off(coords.x, coords.y));
    }

    // now lets look at the next step
    coarse_step = get_next_step(active_step) / constants::PPQN_MAX;

    if (is_step_visible(coarse_step)) {
      int page_relative_coarse_step = get_relative_step(page.rendered, coarse_step);
      mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(page_relative_coarse_step);

      // we want to turn this step ON on the next advance.
      collector->push_back(turn_led_on(coords.x, coords.y));
    }
  };

  bool is_step_visible(int coarse_step) {
    unsigned int page_size = config->mappings.steps.get_area();
    int min_visible_step = (page.rendered * page_size);
    int max_visible_step = ((page.rendered + 1) * page_size) - 1;
    return min_visible_step <= coarse_step && coarse_step <= max_visible_step;
  };
};


#endif
