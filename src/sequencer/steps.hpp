#ifndef SEQUENCER_STEPS_H
#define SEQUENCER_STEPS_H

#include <map>
#include <set>

#include "../io/io.hpp"
#include "../config/config.hpp"
#include "../animation/types.hpp"
#include "../animation/animator.hpp"

#include "types.hpp"


// page_relative_step_t defines a step location relative to the page it is on.
// This is in contrast to a sequence relative step which describes the step
// location with respect to the entire sequence.
struct page_relative_step_t {
  page_idx_t page;
  step_idx_t step;

  bool operator==(const page_relative_step_t &s) const {
    return page == s.page && step == s.step;
  };
};

class Steps {
public:
  page_relative_step_t last   = { .page = 1, .step = 32 };
  bool show_last    = false;

  
  Steps(IO *io, Config *config, Animator *animation)
    : io(io), config(config), animation(animation) {};

  // TODO: we need to be able to
  // (1) render all steps + cursor + last page from scratch
  // (2) render a new cursor without re-rendering all steps (maximally updating two steps, i.e. previous and new)
  // (3) render the last step without re-rending all steps (maximally updating one step)
  // (4) 

  // shows the ui cursor on the next step and hides it on the previous step.
  // the ui cursors are only shown or hidden if they fall on the currently
  // rendered page.
  //
  // this method assumes that if a transition to a new page has occured, the
  // render method has already been called. the side-effect of this assumption
  // is that if the previous cursor was on a page that is no longer the currently
  // rendered page, it has already been removed from the steps ui. thus we only
  // have to remove the previous step from the ui if it was on the currently
  // rendered page.
  void advance_cursor(step_idx_t from, step_idx_t to, page_idx_t rendered_page) {
    page_relative_step_t previous = get_page_relative_step(from);
    page_relative_step_t next = get_page_relative_step(to);

    // only hide the previous cursor if it was on the rendered page.
    if (previous.page == rendered_page) hide_cursor(previous);

    // only show the next cursor if it is on the currently rendered page
    if (next.page == rendered_page) show_cursor(next);
  };
  
  // renders the provided page onto the ui
  void render(page_idx_t page) {
    // remove all animations on page
    animation->remove(config->mappings.steps, led.off);
    
    // render activated steps
    for (auto i : rendered_steps[page]) {
      mapping_coordinates_t c = config->mappings.steps.get_coordinates_from_sequential_index(i);
      monome_led_level_set(io->output.monome, c.x, c.y, led.on);
    }

    // render cursor (only if it is on the current page)

    // render last step if necessary
  };

  // TODO have set + render cursor in caller code within Page
  void render_cursor(int new_cursor) {
    
  };

  void render_cursor(int new_cursor, int old_cursor) {
    
  };

  // sets the cursor position in the sequence
  void set_cursor(int sequence_relative_step) {
    cursor = sequence_relative_steps;

    recompute_collisions();
  };

  // sets the last step position in the sequence
  void set_last_step(int sequence_relative_step) {
    last = sequence_relative_steps;

    recompute_collisions();
  };
  
  // add a rendered step
  void add(int sequence_relative_step) {
    page_relative_step_t page_relative = get_page_relative_step(sequence_relative_step);

    // store in rendered_steps
    rendered_steps[page_relative.page].insert(page_relative.step);

    recompute_collisions();
  };

  // bulk remove multiple rendered steps
  void remove(std::vector<int> sequence_relative_steps) {
    for (int step : sequence_relative_steps) {
      remove(step);
    }
  };

  // remove a rendered step
  void remove(int sequence_relative_step) {
    page_relative_step_t page_relative = get_page_relative_step(sequence_relative_step);
    
    // remove from rendered_steps
    rendered_steps[page_relative.page].erase(page_relative.step);

    recompute_collisions();
  }

  // check whether a provided sequence relative step is rendered.
  bool is_rendered(int sequence_relative_step) {
    page_relative_step_t page_relative = get_page_relative_step(sequence_relative_step);
    return is_rendered(page_relative);
  };

  bool is_rendered(page_relative_step_t page_relative) {
    try {
      std::set<int> rendered_steps_on_page = rendered_steps.at(page_relative.page);
      return rendered_steps_on_page.find(page_relative.step) != rendered_steps_on_page.end();
    } catch (std::out_of_range &error) {
      // there was nothing on that page even apparently
      return false;
    }
  };
  
private:
  // steps rendered on each page
  std::map<int, std::set<int> > rendered_steps;

    // led brightness and animation configuration for pages panel
  struct {
    int on        = 10;
    int off       = 0;
    int cursor    = 15;
    waveform last = {.amplitude.max = 15,
                     .amplitude.min = 4,
                     .modulator = { .type = Unit },
                     .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
    };
    struct {
      waveform last_step_and_cursor          = {.amplitude.max = 15,
                                                .amplitude.min = 4,
                                                .modulator = { .type = Unit },
                                                .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
      };
      waveform last_step_and_step            = {.amplitude.max = 15,
                                                .amplitude.min = 4,
                                                .modulator = { .type = Unit },
                                                .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
      };
      waveform cursor_and_step               = {.amplitude.max = 15,
                                                .amplitude.min = 4,
                                                .modulator = { .type = Unit },
                                                .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
      };
      waveform cursor_and_step_and_last_step = {.amplitude.max = 15,
                                                .amplitude.min = 4,
                                                .modulator = { .type = Unit },
                                                .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
      };
    } collision;
  } led;

  // hide cursor sets the step it falls on back to its orignal state before the
  // cursor was there.
  void hide_cursor(page_relative_step_t cursor) {
    mapping_coordinates_t c = config->mappings.steps.get_coordinates_from_sequential_index(cursor.step);
    
    // first we want to remove any animations that were occuring on this step and
    // set the led state depending on if that step is a rendered step or not
    animations->remove(c, led.on ? is_rendered(cursor) : led.off);

    // add an animation if there is a collision
    if (show_last && is_rendered(last)) animations->add(led.collision.last_step_and_step, c);
  };

  // show cursor sets the step it falls on to display the cursor state
  void show_cursor(page_relative_step_t cursor) {
    mapping_coordinates_t c = config->mappings.steps.get_coordinates_from_sequential_index(cursor.step);
    
    // first we want to remove any animations that were occuring on this step and
    // set the led state to cursor
    animations->remove(c, led.cursor);

    // compute collision detection
    bool last_step_and_cursor_collision          = last == cursor && show_last;
    bool cursor_and_step_collision               = is_rendered(cursor);
    bool cursor_and_step_and_last_step_collision = last_step_and_cursor_collision && cursor_and_step_collision;

    // add animations for collisions
    if (cursor_and_step_and_last_step_collision) {
      animations->add(led.collision.cursor_and_step_and_last_step_collision, c); 
    } else {
      if (last_step_and_cursor_collision) animations->add(led.collisions.last_step_and_cursor, c);
      if (cursor_and_step_collision) animations->add(led.collisions.cursor_and_step, c);
    }
  }
    
  // given a sequence relative step, this converts it to a page relative step.
  page_relative_step_t get_page_relative_step(int sequence_relative_step) {
    return { .page = sequence_relative_step / config->mappings.steps.get_area(),
             .step = sequence_relative_step % config->mappings.steps.get_area(),
    };
  };
  
};

#endif
