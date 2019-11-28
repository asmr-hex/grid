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

  
  //////////////////////////////////////////////
  //                                          //
  //    r e n d e r i n g    m e t h o d s    //
  //                                          //
  //////////////////////////////////////////////
  
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

  // renders the steps on the provided page without a cursor.
  void render(page_idx_t page) {
    render(page, 0, false);
  };

  // renders the steps on a provided page with an optional cursor, if the cursor lies on that page.
  //
  // this method is called upon initial rendering of a page. as it is currently
  // implemented, this method is also called whenever we toggle show_last (in the caller)
  // meaning that even if the last step is on the same page as the currently rendered part,
  // everything on that page will be rendered as opposed to *just* the last step led
  // being re-rendered. this results in inefficient re-rendering only in the case that
  // we decide to show the last step and it happens to be on the currently rendered page.
  //
  // Notes: while this is inefficeint in this one case, it affords us the luxury of not
  // having to include a hide_last_step method and integrating a bunch of hairy logic
  // elsewhere (e.g. advance_cursor, show_last_step, and more probably) since we get the
  // "hide_last_step" functionality for "free" by nature of clearing the rendered steps
  // upon each render.
  //
  // it is worth noting that we *do* incrementally render/unrender the cursor within
  // advance_cursor since this will be called upon each scheduled step if the page is
  // being rendered, resulting in consistently needless re-renders. since showing the
  // last step only happens every once in a while, this inefficiency seems fine to ignore.
  void render(page_idx_t page, step_idx_t cursor_idx, bool render_cursor = true) {
    // remove all animations on page
    animation->remove(config->mappings.steps, led.off);
    
    // render activated steps
    for (auto i : rendered_steps[page]) {
      mapping_coordinates_t c = config->mappings.steps.get_coordinates_from_sequential_index(i);
      monome_led_level_set(io->output.monome, c.x, c.y, led.on);
    }

    // render last step
    if (last.page == page) show_last_step();

    // render cursor
    if (render_cursor) {
      page_relative_step_t cursor = get_page_relative_step(cursor_idx);
      if (cursor.page == page) show_cursor(cursor);
    }
  };

  
  //////////////////////////////////////
  //                                  //
  //    s t a t e    s e t t e r s    //
  //                                  //
  //////////////////////////////////////
  
  // sets the last step position in the sequence
  void set_last_step(int sequence_relative_step) {
    last = get_page_relative_step(sequence_relative_steps);
  };
  
  // add a rendered step
  void add(int sequence_relative_step) {
    page_relative_step_t page_relative = get_page_relative_step(sequence_relative_step);

    // store in rendered_steps
    rendered_steps[page_relative.page].insert(page_relative.step);
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
  }

  
  ////////////////////////////////////////////
  //                                        //
  //    p u b l i c    u t i l i t i e s    //
  //                                        //
  ////////////////////////////////////////////
  
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

  
  //////////////////////////////////////////////
  //                                          //
  //    p r i v a t e    r e n d e r e r s    //
  //                                          //
  //////////////////////////////////////////////
  
  // hide cursor sets the step it falls on back to its orignal state before the
  // cursor was there.
  //
  // this method assumes that the cursor is located onn the page currently being rendered.
  // the caller is responsible for checking if this is the case.
  void hide_cursor(page_relative_step_t cursor) {
    mapping_coordinates_t c = config->mappings.steps.get_coordinates_from_sequential_index(cursor.step);
    
    // first we want to remove any animations that were occuring on this step and
    // set the led state depending on if that step is a rendered step or not
    animations->remove(c, led.on ? is_rendered(cursor) : led.off);

    // add an animation if there is a collision
    if (show_last && is_rendered(last)) animations->add(led.collision.last_step_and_step, c);
  };

  // show cursor sets the step it falls on to display the cursor state
  //
  // this method assumes that the cursor is located onn the page currently being rendered.
  // the caller is responsible for checking if this is the case.
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

  // shows the last step unless the state tells us we shouldn't be showing the last step.
  //
  // this method assumes that the last step is located onn the page currently being rendered.
  // the caller is responsible for checking if this is the case.
  void show_last_step() {
    // return early if we have no business showing the last step
    if (!show_last) return;
    
    mapping_coordinates_t c = config->mappings.steps.get_coordinates_from_sequential_index(last.step);

    // first we want to remove any animations that were occuring on this step and
    // set the led state depending on if that step is a rendered step or not
    animations->remove(c, led.on ? is_rendered(last) : led.off);

    // add animations on collision
    if (is_rendered(last)) animations->add(led.collision.last_step_and_step, c);
  };


  //////////////////////////////////////////////
  //                                          //
  //    p r i v a t e    u t i l i t i e s    //
  //                                          //
  //////////////////////////////////////////////
  
  // given a sequence relative step, this converts it to a page relative step.
  page_relative_step_t get_page_relative_step(int sequence_relative_step) {
    return { .page = sequence_relative_step / config->mappings.steps.get_area(),
             .step = sequence_relative_step % config->mappings.steps.get_area(),
    };
  };
  
};

#endif
