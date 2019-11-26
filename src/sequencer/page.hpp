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

  // location of cursor, page relative
  int cursor;

  Page(IO *io, Config *config, Animator *animation)
    : io(io), config(config), animation(animation) {};

  // render everything on a provided page
  void render(int page) {
    rendered = page;

    render_page(page);
    render_selection_panel(page);
  };

  // render everything on the page_under edit
  void render() {
    render(under_edit);
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

  // steps rendered on each page
  std::map<int, std::set<int> > rendered_steps;

  // renders the activated steps on the sequence page as well as the cursor and last step
  // if appropriate.
  void render_page(int page) {
    // remove all animations
    
    // render activated steps

    // render cursor

    // render last step if necessary
        
  };

  // renders the page selection panel.
  void render_selection_panel(int page) {
    
  };
};

#endif
