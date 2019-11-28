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

  bool follow_cursor = false;

  Steps *steps;
  
  Page(IO *io, Config *config, Animator *animation)
    : io(io), config(config), animation(animation) {
    steps = new Steps(io, config, animation);
  };


  void advance_cursor(step_idx_t from, step_idx_t to) {
    // get the page of the next cursor
    page_idx_t page_to = get_page(to);

    in_playback = page_to;

    if (page_to != rendered) {
      if (follow_cursor) {
        under_edit = in_playback;
        // render steps
        if (!steps->show_last) steps->render(page_to, to);
      }

      // render only page selection ui
      render(page_to, to, false);
    }
    
    steps->advance_cursor(from, to, rendered);
  };

  // render everything on the page_under edit
  void render(step_idx_t cursor) {
    render(under_edit, cursor);
  };
  
  // render the page provided.
  //
  // specifically, render the page selection ui and, unless specified, render
  // the steps ui (which includes the activated steps, cursor (if is currently
  // on the page), and the last step (if it is currently on the page and being shown)).
  void render(page_idx_t page, step_idx_t cursor, bool render_steps = true) {
    // only render steps if the provided page isn't already rendered and we
    // wish to render the steps.
    if (rendered != page && render_steps) {
      steps->render(page, cursor);
    }

    // set the provided page as the rendered page
    rendered = page;

    // render the page selection panel
    render_pages_panel();
  };

  void toggle_follow_cursor() {
    follow_cursor = !follow_cursor;
  };

  void set_show_last_step(bool show = true) {
    steps->show_last = show;
  };
  
  // sets the provided page as the page under_edit
  void edit(page_idx_t page) {
    under_edit = page;
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
    struct {
      waveform in_playback_and_rendered = {.amplitude.max = 15,
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

  void render_pages_panel() {
    int max_page_idx = config->mappings.pages.get_area() - 1;

    // get coordinates of in_playback and rendered pages
    mapping_coordinates_t in_playback_c = config->mappings.pages.get_coordinates_from_sequential_index(in_playback);
    mapping_coordinates_t rendered_c = config->mappings.pages.get_coordinates_from_sequential_index(rendered);

    // get coordinate regions for before last step and after last step
    std::vector<mapping_coordinates_t> cb = config->mappings.pages.get_region_coordinates(0, last);
    std::vector<mapping_coordinates_t> ca = config->mappings.pages.get_region_coordinates(last + 1, max_page_idx);

    // remove animations for all pages before last page and set led appriopriately
    animation->remove(cb, led.within_bounds);

    // remove animations for all pages after last page and set led appriopriately
    animation->remove(ca, led.out_of_bounds);

    // check for collisions between in_playback and rendered page
    if (rendered == in_playback) {
      animation->add(led.collision.in_playback_and_rendered, rendered_c);
    } else {
      animation->add(led.in_playback, in_playback_c);
      monome_led_level_set(io->output.monome, rendered_c.x, rendered_c.y, led.under_edit);
    }
  };
  
  //////////////////////////////////////////////
  //                                          //
  //    p r i v a t e    u t i l i t i e s    //
  //                                          //
  //////////////////////////////////////////////
  
  page_idx_t get_page(step_idx_t step) {
    unsigned int page_size = config->mappings.steps.get_area();
    return step / page_size;
  };
};

#endif
