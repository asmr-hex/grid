#ifndef SEQUENCER_PAGES_H
#define SEQUENCER_PAGES_H

#include <map>
#include <set>

#include "../io/io.hpp"
#include "../config/config.hpp"
#include "../animation/types.hpp"
#include "../animation/animator.hpp"

#include "types.hpp"
#include "steps.hpp"


class Page {
public:
  page_idx_t rendered    = 0;
  page_idx_t under_edit  = 0;
  page_idx_t in_playback = 0;
  page_idx_t last        = 1;

  Page(IO *io, Config *config, Animator *animation)
    : io(io), config(config), animation(animation) {
    steps = new Steps(op, config, animation);
  };

  // TODO in terms of rendering, we need to be able to do the following
  // 1) render steps, last step, cursor from scratch
  // 2) "incrementally" render last step without re-rendering all steps
  // 3) "incrementally" render cursor without re-rendering all steps

  void advance_cursor(step_idx_t from, step_idx_t to) {
    // if we maintain cursor state within steps, we need to
    // (1) know the current page being rendered
    // (2) know if the part is even being rendered
    // (3) somehow change the state of the in_playback page in page from within steps...

    // if we pass in the cursor (or active step) from Part
    // (1) we can update in_playback part in Page as it passes through
    // (2) determine to advance the steps cursor only if rendered == in_playback within Page
    // (3)
    //
    // i think threading through the cursor from Part is the way to go. means we only have
    // to maintain that state in one place (good!) and it fits this architecture pretty well.
    //
    // BUT question: do we need to pass cursor to these render methods?
    // i think we should....

    // update the page in playback given the next step
    page_idx_t page_to = get_page(to);

    // TODO re-render page selection panel if the in_playback page differs from the page_to

    // TODO do something about follow step
    
    steps->advance_cursor(from, to, rendered);
  };
  
  // render everything on a provided page
  void render(int page) {
    rendered = page;

    steps->render(page);

    // do remaining rendering of page panel
  };

  // render everything on the page_under edit
  void render() {
    render(under_edit);
  };

  // sets the provided page as the page under_edit
  void edit(int page) {
    
  };
  
private:
  IO *io;
  Config *config;
  Animator *animation;

  Steps *steps;
  
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
    struct {
      waveform in_playback_and_rendered = {.amplitude.max = 15,
                                           .amplitude.min = 4,
                                           .modulator = { .type = Unit },
                                           .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
      };
    } collision;
  } led;
  
};

#endif
