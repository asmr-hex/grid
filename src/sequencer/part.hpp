#ifndef PART_H
#define PART_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "utils.hpp"
#include "types.hpp"
#include "constants.hpp"


class Part {
public:
  Part(int id) : id(id) {
    // load part file if it exists.
    active_step = -1; // we want to initialize active_step to -1 whenever we are starting the sequence from the beginning
    ppqn = 2;
    length = 32;
    page = 0;
    
    // TODO remove this test
    // step_event_t se1 = { OSC, 0x0000, std::vector<unsigned char>(0x00FF) };
    // step_event_t se2 = { OSC, 0x0100, std::vector<unsigned char>(0x00FF) };
    // std::map<event_uid_t, step_event_t> m1;
    // m1[0x0000] = se1;
    // std::map<event_uid_t, step_event_t> m2;
    // m2[0x0001] = se2;
    // sequence[0] = m1;
    // sequence[1*constants::PPQN] = m2;
  };

  void load(int new_part_idx) {
    
  };

  void save() {
    
  };
  
  std::vector<step_event_t> advance(bool instrument_is_displayed) {
    std::vector<step_event_t> next_events;

    // should we display the step in the ui?
    std::vector<step_event_t> ui_events = get_next_ui_step_events(instrument_is_displayed);
    for (step_event_t e : ui_events) {
      next_events.push_back(e);
    }
    
    // if playback is not playing, do not advance.
    
    // if ppqn = 1, we want step_size = 1
    // if ppqn = 2, we want ste_size = 2;
    // etc.
    
    // advance to next step
    active_step = get_next_step(active_step);

    try {
      // get events from new active step
      auto step = sequence.at(active_step);

      for (auto i : step) {
        // copy sequence event into results
        next_events.push_back(i.second);
      }
    } catch (std::out_of_range &error) {
      // this step is empty. carry on.
    }

    return next_events;
  };
  
private:
  int id;
  int ppqn;
  int length;
  int active_step;
  int page;
  bool unsaved;
  sequence_t sequence;
  playback_t playback;

  void load_from_file(std::string filepath) {
    // internally load
  }

  int get_next_step(int step) {
    // advance to next step
    step += ppqn;

    // if the active step is now greater than the last step, circle back
    if (step > (length * constants::PPQN) - 1) {
      step = 0;
    }

    return step;
  };
  
  std::vector<step_event_t> get_next_ui_step_events(bool instrument_is_displayed) {
    std::vector<step_event_t> next_ui_events;

    // escape immediately if this is not the currently active isntrument.
    if (!instrument_is_displayed) return next_ui_events;

    int hardware_step = active_step / constants::PPQN;

    if (step_visible_in_ui(hardware_step)) {
      // this step is visible.

      unsigned int *r = get_grid_coordinates_of(hardware_step);
      unsigned int x = r[0];
      unsigned int y = r[1];
      
      // we want to turn this step off on the next advance.
      next_ui_events.push_back(turn_led_off(x, y));
    }

    // now lets look at the next step
    hardware_step = get_next_step(active_step) / constants::PPQN;

    if (step_visible_in_ui(hardware_step)) {
      unsigned int *r = get_grid_coordinates_of(hardware_step);
      unsigned int x = r[0];
      unsigned int y = r[1];

      // we want to turn this step off on the next advance.
      next_ui_events.push_back(turn_led_on(x, y));      
    }

    return next_ui_events;
  };

  bool step_visible_in_ui(int hardware_step) {
    int min_visible_step = (page * constants::SEQUENCE_PAGE_SIZE);
    int max_visible_step = (page * constants::SEQUENCE_PAGE_SIZE) + constants::SEQUENCE_PAGE_SIZE;
    return min_visible_step <= hardware_step && hardware_step <= max_visible_step;
  };
  
  unsigned int *get_grid_coordinates_of(int hardware_step) {
    unsigned int xy[2];
    xy[0] = hardware_step % constants::SEQUENCE_COL_SIZE;
    xy[1] = hardware_step / constants::SEQUENCE_COL_SIZE;
    return xy;
  };
  
};


#endif
