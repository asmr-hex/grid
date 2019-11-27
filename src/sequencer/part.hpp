#ifndef PART_H
#define PART_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>

#include "utils.hpp"
#include "types.hpp"
#include "sequence.hpp"
#include "constants.hpp"

#include "../io/io.hpp"

#include "../animation/types.hpp"
#include "../animation/animator.hpp"

#include "../config/config.hpp"
#include "../config/mappings/types.hpp"
#include "../handlers/utils.hpp"

#include "ppqn.hpp"
#include "transport.hpp"


class Part {
public:
  struct {
    int part;
    int bank;
  } id;
  Ppqn *ppqn;
  Transport *transport;
  bool is_rendered = false;
  struct {
    int rendered = 0;
    int under_edit = 0;
    int last_step = 1; // refactor to of_last_step
    int cursor = 0;
  } page;
  bool follow_cursor = false;
  int length = 32; // TODO refactor this to be last_step
  bool show_last_step = false;
  bool unsaved;

  
  Part(int part_id,
       int bank_id,
       Config *config,
       IO *io,
       Animator *animation,
       std::function<void (int, int)> swap_part_in_playback)
    : config(config),
      io(io),
      animation(animation),
      swap_part_in_playback(swap_part_in_playback) {
    // load part file if it exists.
    id.part = part_id;
    id.bank = bank_id;

    ppqn = new Ppqn(io, animation, config);
    transport= new Transport(id.part, id.bank, io, animation, config, swap_part_in_playback);
    
    length = 64;
    default_note = "C5";
  };

  //////////////////////////////////////////////
  //                                          //
  //    t r a n s p o r t    m e t h o d s    //
  //                                          //
  //////////////////////////////////////////////
  
  // prepare to play part and possibly render
  void play() {
    transport->prepare_to_play();
    if (is_rendered) transport->render();
  };

  // pause part and render and possibly render
  void pause() {
    transport->pause();
    if (is_rendered) transport->render();
  };

  // stop part and possibly render
  void stop() {
    transport->stop();
    active_step = 0;
    if (is_rendered) transport->render();
  };

  // transition to a new part to play
  //
  // when we are transitioning to a new part, the part currently in playback
  // is never being rendered, so we don't give the option to render here.
  void transition_to(Part *next_part) {
    transport->transition_to(next_part->transport);
  };

  //////////////////////////////////////////////
  //                                          //
  //    s c h e d u l e d    m e t h o d s    //
  //                                          //
  //////////////////////////////////////////////
  
  void sync_to_clk(int pulse) {
    if (pulse == 0) {
      // perform on beat updates if appropriate
      on_beat_updates();
    }

    on_pulse_updates(pulse);
  };
  
  void advance_ui_cursor() {
    // do not advance if we are not playing!
    if (!playback.is_playing) return;
    
    int coarse_step = active_step / constants::PPQN_MAX;
    bool current_step_is_visible = is_step_visible(coarse_step);

    if (current_step_is_visible && !is_step_on(coarse_step)) {
      int page_relative_coarse_step = get_relative_step(page.rendered, coarse_step);
      mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(page_relative_coarse_step);
      
      // we want to turn this step OFF on the next advance.
      monome_led_off(io->output.monome, coords.x, coords.y);
    }

    // now lets look at the next step
    int next_coarse_step = get_next_step(active_step) / constants::PPQN_MAX;
    bool next_step_is_visible = is_step_visible(next_coarse_step);

    if (next_step_is_visible) {
      int page_relative_coarse_step = get_relative_step(page.rendered, next_coarse_step);
      mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(page_relative_coarse_step);

      // we want to turn this step ON on the next advance.
      monome_led_on(io->output.monome, coords.x, coords.y);
    }

    // update the cursor's page if necessary
    int page_of_next_step = get_page(next_coarse_step);
    if (page_of_next_step != page.cursor) {
      page.cursor = get_page(next_coarse_step);

      // render the next page if necessary
      if (follow_cursor) {
      
        // render cursor page only if we aren't showing the last step
        if (!show_last_step) render_page(page.cursor);

        // set page under edit as cursor page
        page.under_edit = page.cursor;
      }
      
      // update the page UI
      render_page_selection_ui();
    }
  };

  // advances the ui cursor of this part from the current step to the next step.
  //
  // this method assumes that the current part is under_edit and the instrument
  // that owns it is also under_edit (e.g. this part is currently being rendered).
  // checks for those conditions are the responsibility of the caller.
  void advance_cursor() {
    // convert active_step, which is a granular step, to a normal step
    step_idx_t from = active_step / constants::PPQN_MAX;
    step_idx_t to = get_next_step(active_step) / constants::PPQN_MAX;
    
    page->advance_cursor(from, to);
  };
  
  std::vector<step_event_t> advance() {
    std::vector<step_event_t> next_events;
    
    // do not advance if we are not playing! just return empty vector!
    if (!playback.is_playing) return next_events;
    

    // get all next events
    sequence.collect_all_events_at(active_step, &next_events);

    // advance to next step
    active_step = get_next_step(active_step);

    if ( active_step == 0 ) {
      // we have reached the end of a sequence cycle
      on_cycle_updates();
    }
    
    return next_events;
  };
  
  // add a step to the sequence. this adds the default midi note to the sequence.
  void add_step(unsigned int coarse_step_idx) {
    int abs_coarse_step = get_absolute_step(page.under_edit, coarse_step_idx);
    step_event_t event = midi_note_on(default_note, 0, 127);
    step_idx_t step = get_fine_step_index(abs_coarse_step);

    // TODO ....we should probably be using a mutex... -___-
    sequence.add_midi_note_on_event(event, step, 1);

    // add step to rendered steps
    rendered_steps[page.under_edit].insert(coarse_step_idx);
  };
  
  void remove_step(unsigned int coarse_step_idx) {
    int abs_coarse_step = get_absolute_step(page.under_edit, coarse_step_idx);
    step_idx_t step = get_fine_step_index(abs_coarse_step);
    sequence.remove_step(step);

    // remove step from rendered steps
    rendered_steps[page.under_edit].erase(coarse_step_idx);
  };

  // sets the ppqn given a sequential index.
  //
  // if the part is currently playing, set the next ppqn to be scheduled
  // otherwise, immediately set the current ppqn.
  void set_ppqn(int idx) {
    if (playback.is_playing) {
      ppqn->set_next(idx);
    } else {
      ppqn->set(idx);
    }

    if (is_rendered) ppqn->render();
  };
  
  // renders a page onto the ui without forced re-rendering
  void render_page(int new_page) {
    render_page(new_page, false);
  };

  // renders a page onto the ui
  void render_page(int new_page, bool force_rerender) {
    if (page.rendered != new_page || force_rerender) {
      page.rendered = new_page;

      // render all visibile steps on this page
      set_led_region_intensity(io, &config->mappings.steps, 0);
      for (auto i : rendered_steps[page.rendered]) {
        mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(i);
        monome_led_on(io->output.monome, coords.x, coords.y);
      }      
    }

    // if we are rendering the pafge with the last step, show or hide last step
    if (page.last_step == new_page) {
      int relative_last_step = get_relative_step(new_page, length -1);
      mapping_coordinates_t coords = config->mappings.steps.get_coordinates_from_sequential_index(relative_last_step);
      
      if (show_last_step) {
        // show the last step
        monome_led_on(io->output.monome, coords.x, coords.y);  
      } else if (!show_last_step && !is_step_on(get_last_step())) {
        // hide the last step unless that step is on
        monome_led_off(io->output.monome, coords.x, coords.y);
      }
    }
  };

  void render_page_selection_ui() {
    render_page_selection_ui(page.last_step);
  };
  
  // renders the page selection panel in the ui (monome grid).
  //
  // there should typically be two pages highlighted at a given time:
  //  1) the currently rendered page
  //  2) the page the cursor is on
  // when these pages are the same, there will only be one page highlighted.
  void render_page_selection_ui(int last_step_page) {
    // set all page leds before last page
    set_led_region_intensity(io,
                             config->mappings.pages.get_region_coordinates(0, last_step_page),
                             led_brightness.page.within_last_step);
    // set all page leds after last page
    set_led_region_intensity(io,
                             config->mappings.pages.get_region_coordinates(last_step_page + 1,
                                                                           config->mappings.pages.get_area() - 1),
                             led_brightness.page.beyond_last_step);

    // remove all animations from pages
    for (mapping_coordinates_t m : config->mappings.pages.get_region_coordinates(0, last_step_page)) {
      animation->remove(m);
    }
    
    // set rendered page to highlighted
    mapping_coordinates_t rendered_page_coords = config->mappings.pages.get_coordinates_from_sequential_index(page.rendered);
    monome_led_level_set(io->output.monome,
                         rendered_page_coords.x,
                         rendered_page_coords.y,
                         led_brightness.page.under_edit);

    if (!follow_cursor) {
      if (page.rendered != page.cursor) {
        // set cursor page to highlighted
        mapping_coordinates_t cursor_page_coords = config->mappings.pages.get_coordinates_from_sequential_index(page.cursor);
        waveform w = {.amplitude.max = led_brightness.page.within_last_step,
                      .amplitude.min = led_brightness.page.beyond_last_step,
                      .modulator = { .type = Unit },
                      .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
        };
        animation->add(w, cursor_page_coords);
      } else {
        waveform w = {.amplitude.max = led_brightness.page.under_edit,
                      .amplitude.min = led_brightness.page.in_playback,
                      .modulator = { .type = Unit },
                      .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
        };
        animation->add(w, rendered_page_coords);
      } 
    }
  };

  void render_last_step_ui() {
    if (follow_cursor) {
      waveform w = { .amplitude.max = 9,
                     .amplitude.max = 4,
                     .modulator = { .type = Unit },
                     .pwm = { .duty_cycle = 0.5, .period = 400, .phase = 0 }
      };
      animation->add(w, config->mappings.last_step);
    } else {
      // remove any pre-existing animations
      animation->remove(config->mappings.last_step);
    }
    
    if (show_last_step) {
      monome_led_on(io->output.monome, config->mappings.last_step.x, config->mappings.last_step.y);
    } else if (!show_last_step && !follow_cursor) {
      monome_led_off(io->output.monome, config->mappings.last_step.x, config->mappings.last_step.y); 
    }
  }
  
  // updates the state of last step of a part and updates the ui (monome grid).
  //
  // this function assumes that the currently rendered page is the page on which
  // the new last step lies. this is important because the coordinates supplied for
  // the new last step are relative to the currently rendered page.
  void update_last_step(mapping_coordinates_t grid) {
    int new_last_step = get_absolute_step(page.rendered, config->mappings.steps.get_sequential_index_from_coordinates(grid.x, grid.y));

    // turn off led for outdated last step if it was on the same page as the new last step
    // but only if the outdated last step was not already on as an active step.
    if (page.last_step == page.rendered && !is_step_on(get_last_step())) {
      // TODO if there is a step activated on the last step, we need to differentiate...

      mapping_coordinates_t old_last_step = config->mappings.steps.get_coordinates_from_sequential_index(get_relative_step(page.rendered, get_last_step()));
      monome_led_off(io->output.monome, old_last_step.x, old_last_step.y);
    }

    // update the state for the last step (length)
    length = new_last_step + 1;

    // update the page on which the last step lies. again, we assume this is the currently
    // rendered page.
    page.last_step = page.rendered;

    // turn on the led for the new last step.
    monome_led_on(io->output.monome, grid.x, grid.y);
  };

  bool is_step_on(unsigned int coarse_step_idx) {
    step_idx_t step = get_fine_step_index(coarse_step_idx);
    return sequence.is_step_on(step, active_layer);
  };

  int get_last_step() {
    return length - 1;
  };
  
  int get_page_relative_last_step(int page_i) {
    return get_relative_step(page_i, get_last_step());
  };

  int get_absolute_step(int page_i, int page_relative_step) {
    return (config->mappings.steps.get_area() * page_i) + page_relative_step;
  };

  int get_relative_step(int page_i, int page_absolute_step) {
    return page_absolute_step - (config->mappings.steps.get_area() * page_i);
  };
    
private:
  granular_step_idx_t active_step = 0;
  event_uid_t active_layer = 0x0000;  // 0x0000 is the 'all' layer
  Sequence sequence;
  std::string default_note;
  
  IO *io;
  Config *config;
  Animator *animation;
  std::map<int, std::set<int> > rendered_steps;
  std::function<void (int, int)> swap_part_in_playback;

  struct {
    struct {
      int beyond_last_step = 0;
      int within_last_step = 5;
      int under_edit = 15;
      int in_playback = 10;
    } page;
  } led_brightness;

  // given a current granular step, this computes the next granular step in the
  // current part.
  //
  // this takes into account rolling over from the last step to the first step.
  granular_step_idx_t get_next_step(granular_step_idx_t step) {
    // advance to next step
    step += ppqn->current;

    // if the active step is now greater than the last step, loop back
    if (step > (length * constants::PPQN_MAX) - 1) {
      step = 0;
    }

    return step;
  };

  //////////////////////////////////////////////////
  //                                              //
  //  a s y n c     s t a t e     u p d a t e s   //
  //                                              //
  //////////////////////////////////////////////////
  
  void on_cycle_updates() {
    // relinquish playback status and give the next part a turn!
    if (transport->is_stopping && transport->is_transitioning) {
      transport->handoff_playback();
      if (is_rendered) transport->render();
    }
  }
  
  void on_beat_updates() {
    // if we are about to begin playing, make sure it starts on the beat
    if (transport->is_about_to_start) {
      transport->play();
      if (is_rendered) transport->render();
    }

    // if we are changing the ppqn, wait until beat for it to take effect
    if (ppqn->pending_change) {
      ppqn->set();
      if (is_rendered) ppqn->render();
    }
  }

  void on_pulse_updates(int pulse) {
    // if we are about to pause, make sure we pause on the beat so we can resume on the beat
    if (pulse == transport->pause_pulse_offset && transport->is_about_to_unpause) {
      transport->play();
      if (is_rendered) transport->render();
    }
  };
  
  bool is_step_visible(int coarse_step) {
    unsigned int page_size = config->mappings.steps.get_area();
    int min_visible_step = (page.rendered * page_size);
    int max_visible_step = ((page.rendered + 1) * page_size) - 1;
    return min_visible_step <= coarse_step && coarse_step <= max_visible_step;
  };
};


#endif
