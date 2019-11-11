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


class Part {
public:
  Part(int id) : id(id) {
    // load part file if it exists.
    ppqn = 8;
    length = 32;
    page = 0;
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
    step_event_t event = midi_note_on(default_note, 0, 127);
    step_idx_t step = get_fine_step_index(coarse_step_idx);

    // TODO ....we should probably be using a mutex... -___-
    sequence.add_midi_note_on_event(event, step, 1);
  };
  
  void remove_step(unsigned int coarse_step_idx) {
    step_idx_t step = get_fine_step_index(coarse_step_idx);
    sequence.remove_step(step);
  };
  
  int ppqn;
  int page;
  int length;
  bool unsaved;
  
private:
  int id;
  int active_step;
  event_uid_t active_layer = 0x0000;  // 0x0000 is the 'all' layer
  Sequence sequence;
  std::string default_note;

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

    int hardware_step = active_step / constants::PPQN_MAX;

    if (step_visible_in_ui(hardware_step) && !is_step_on(hardware_step)) {
      // this step is visible.

      unsigned int *r = get_grid_coordinates_of(hardware_step);
      unsigned int x = r[0];
      unsigned int y = r[1];
      
      // we want to turn this step off on the next advance.
      collector->push_back(turn_led_off(x, y));
    }

    // now lets look at the next step
    hardware_step = get_next_step(active_step) / constants::PPQN_MAX;

    if (step_visible_in_ui(hardware_step)) {
      unsigned int *r = get_grid_coordinates_of(hardware_step);
      unsigned int x = r[0];
      unsigned int y = r[1];

      // we want to turn this step off on the next advance.
      collector->push_back(turn_led_on(x, y));      
    }
  };

  bool step_visible_in_ui(int hardware_step) {
    int min_visible_step = (page * constants::SEQUENCE_PAGE_SIZE);
    int max_visible_step = (page * constants::SEQUENCE_PAGE_SIZE) + constants::SEQUENCE_PAGE_SIZE;
    return min_visible_step <= hardware_step && hardware_step <= max_visible_step;
  };

  // TODO remove this in favor of get_coordinates_from_sequential_index
  unsigned int *get_grid_coordinates_of(int hardware_step) {
    unsigned int xy[2];
    xy[0] = hardware_step % constants::SEQUENCE_COL_SIZE;
    xy[1] = hardware_step / constants::SEQUENCE_COL_SIZE;
    return xy;
  };

};


#endif
