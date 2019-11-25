#ifndef SEQUENCER_PPQN_H
#define SEQUENCER_PPQN_H

#include "constants.hpp"
#include "../io/io.hpp"
#include "../animation/animator.hpp"
#include "../config/config.hpp"


class Ppqn {
public:
  int current = 8;
  int next;
  bool pending_change = false;


  Ppqn(IO *io, Animator *animation, Config *config)
    : io(io), animation(animation), config(config) {};

  // render the ppqn selection panel in the ui.
  void render() {
    int current_idx = index_from_ppqn(current);
    int next_idx = index_from_ppqn(next);
    mapping_coordinates_t current_coords = config->mappings.ppqn.get_coordinates_from_sequential_index(current_idx);
    mapping_coordinates_t next_coords = config->mappings.ppqn.get_coordinates_from_sequential_index(next_idx);

    // turn off all leds in ppqn zone
    set_led_region_intensity(io, &config->mappings.ppqn, 0);
    
    // turn on the current ppqn
    monome_led_on(io->output.monome, current_coords.x, current_coords.y);
  };

  // set the next ppqn from the provided ppqn index and re-render ui.
  //
  // this doesn't change the current ppqn but updates the ppqn which will be
  // set next. this method is usually called from within the event handlers thread.
  void set_next(int idx) {    
    // if a change is already pending, remove the animation for the stale next ppqn
    if (pending_change) {
      animation->remove(config->mappings.ppqn.get_coordinates_from_sequential_index(index_from_ppqn(next)), 0);
      std::cout << next << " ANIMATION OFF (in set_next)\n";
    }

    next = ppqn_from_index(idx);
    pending_change = true;

    // set animation for next ppqn
    waveform w = {.amplitude.max = 9,
                  .modulator = { .type = Unit },
                  .pwm = { .duty_cycle = 0.1, .period = 100, .phase = 0 }
    };
    animation->add(w, config->mappings.ppqn.get_coordinates_from_sequential_index(index_from_ppqn(next)));

    std::cout << next << " ANIMATION ON\n";
    
    render();
  }

  // sets the current ppqn using the next ppqn and re-renders.
  //
  // we assume that next has been set already using set_next method. it is worth noting
  // that this method variant is typically called from within a different thread as the
  // set_next method. this method is called from the sequencer scheduler thread usually.
  void set() {
    // remove the animation for the next ppqn and turn off pending change.
    animation->remove(config->mappings.ppqn.get_coordinates_from_sequential_index(index_from_ppqn(next)), 0);
    std::cout << next << " ANIMATION OFF (in set)\n";
    
    set(index_from_ppqn(next));
  }

  // sets the current ppqn using the provided ppqn index and re-renders.
  void set(int idx) {      
    pending_change = false;
    current = ppqn_from_index(idx);

    render();
  };
  
private:
  IO *io;
  Config *config;
  Animator *animation;
  
  int ppqn_from_index(int idx) {
    switch (idx) {
    case 0:
      return constants::PPQN::One;
    case 1:
      return constants::PPQN::Two;
    case 2:
      return constants::PPQN::Four;
    case 3:
      return constants::PPQN::Eight;
    case 4:
      return constants::PPQN::Sixteen;
    case 5:
      return constants::PPQN::ThirtyTwo;
    case 6:
      return constants::PPQN::SixtyFour;
    default:
      return constants::PPQN::Eight;
    }
  }

  int index_from_ppqn(int ppqn) {
    switch (ppqn) {
    case constants::PPQN::One:
      return 0;
    case constants::PPQN::Two:
      return 1;
    case constants::PPQN::Four:
      return 2;
    case constants::PPQN::Eight:
      return 3;
    case constants::PPQN::Sixteen:
      return 4;
    case constants::PPQN::ThirtyTwo:
      return 5;
    case constants::PPQN::SixtyFour:
      return 6;
    default:
      return 0;
    }
  }
};

#endif
