#ifndef SEQUENCER_PAGES_H
#define SEQUENCER_PAGES_H

#include <map>
#include <set>

#include "../io/io.hpp"
#include "../config/config.hpp"
#include "../animation/types.hpp"
#include "../animation/animator.hpp"


class Page {
public:
  int rendered    = 0;
  int under_edit  = 0;
  int in_playback = 0;
  int last        = 1;

  Pages(IO *io, Config *config, Animator *animation)
    : io(io), config(config), animation(animation) {};

  void render() {
    
  };
  
private:
  IO *io;
  Config *config;
  Animator *animation;

  // led brightness and animation configuration for pages panel
  struct {
    int out_of_bounds             = 0;
    int within_bounds             = 4;
    int under_edit                = 15;
    waveform in_playback          = {.amplitude.max = 4,
                                     .amplitude.min = 0,
                                     .modulator = { .type = Unit },
                                     .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
    };
    waveform in_playback_rendered = {.amplitude.max = 15,
                                     .amplitude.min = 4,
                                     .modulator = { .type = Unit },
                                     .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
    };
  } led;
  
  std::map<int, std::set<int> > rendered_steps;
};

#endif
