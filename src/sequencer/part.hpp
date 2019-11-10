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
    default_note = "C4";
    
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
      // TODO: we will need to refactor this to come up with a way to
      // gracefully handle midi off events when we change sequence length!

      // get events from new active step
      auto step = sequence.at(active_step);

      for (auto i : step) {
        // copy sequence event into results
        next_events.push_back(i.second);

        std::cout << "YAY @step=0\n";
      }
    } catch (std::out_of_range &error) {
      // this step is empty. carry on.

      if (active_step == 0) {
        std::cout << "NOTHING @step=0\n";
      }
    }

    return next_events;
  };

  bool is_step_on(unsigned int coarse_step_idx) {
    try {
      sequence.at(get_fine_step_index(coarse_step_idx));
      return true;
    } catch (std::out_of_range &error) {
      return false;
    }
  };

  /*
    add a step to the sequence. this adds the default midi note to
    the sequence.
   */
  void add_step(unsigned int coarse_step_idx) {
    step_event_t midi_note_on_event = midi_note_on(default_note, 0, 127);
    step_event_t midi_note_off_event = midi_note_off(default_note, 0);
    step_idx_t step_on = get_fine_step_index(coarse_step_idx);
    step_idx_t step_off = get_fine_step_index(coarse_step_idx + 1);

    // TODO ....we should probably be using a mutex... -___-
    sequence[step_on][midi_note_on_event.id] = midi_note_on_event;
    sequence[step_off][midi_note_off_event.id] = midi_note_off_event;
  };
  
  void remove_step(unsigned int step_idx) {
    try {
      // TODO will this handle dropping stuff from memory? will this cause a mem leak?
      sequence.erase(get_fine_step_index(step_idx));
    } catch (std::out_of_range &error) {
      // a step should ALWAYS exist if this is called...so this shouldn't happen...
      // TODO add logging...
    }
  };
  
  int ppqn;
  int page;
  int length;
  bool unsaved;
  
private:
  int id;
  int active_step;
  sequence_t sequence;
  playback_t playback;
  std::string default_note;

  void load_from_file(std::string filepath) {
    // internally load
  }

  int get_next_step(int step) {
    // advance to next step
    step += ppqn;

    // if the active step is now greater than the last step, circle back
    if (step > (length * constants::PPQN_MAX) - 1) {
      step = 0;
    }

    return step;
  };
  
  std::vector<step_event_t> get_next_ui_step_events(bool instrument_is_displayed) {
    std::vector<step_event_t> next_ui_events;

    // escape immediately if this is not the currently active isntrument.
    if (!instrument_is_displayed) return next_ui_events;

    int hardware_step = active_step / constants::PPQN_MAX;

    if (step_visible_in_ui(hardware_step) && !is_step_on(hardware_step)) {
      // this step is visible.

      unsigned int *r = get_grid_coordinates_of(hardware_step);
      unsigned int x = r[0];
      unsigned int y = r[1];
      
      // we want to turn this step off on the next advance.
      next_ui_events.push_back(turn_led_off(x, y));
    }

    // now lets look at the next step
    hardware_step = get_next_step(active_step) / constants::PPQN_MAX;

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

  // TODO remove this in favor of get_coordinates_from_sequential_index
  unsigned int *get_grid_coordinates_of(int hardware_step) {
    unsigned int xy[2];
    xy[0] = hardware_step % constants::SEQUENCE_COL_SIZE;
    xy[1] = hardware_step / constants::SEQUENCE_COL_SIZE;
    return xy;
  };

  step_idx_t get_fine_step_index(unsigned int coarse_step_index) {
    return (coarse_step_index * constants::PPQN_MAX) - 1;
  };
};


#endif
