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
#include "page.hpp"
#include "transport.hpp"


class Part {
public:
  struct {
    int part;
    int bank;
  } id;
  Ppqn *ppqn;
  Page *page;
  Transport *transport;
  bool is_rendered = false;

  int length = 32; // TODO refactor this to be last_step
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
    page = new Page(io, config, animation);
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
    transport->pause(active_step % constants::PPQN_MAX);
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

  // hook for executing code on specific beats.
  void sync_to_clk(int pulse) {
    if (pulse == 0) {
      // perform on beat updates if appropriate
      on_beat_updates();
    }

    on_pulse_updates(pulse);
  };
  
  // advances the ui cursor of this part from the current step to the next step.
  //
  // this method assumes that the current part is under_edit and the instrument
  // that owns it is also under_edit (e.g. this part is currently being rendered).
  // checks for those conditions are the responsibility of the caller.
  void advance_cursor() {
    // convert active_step, which is a granular step, to a normal step
    step_idx_t from = get_cursor();
    step_idx_t to = get_next_step(active_step) / constants::PPQN_MAX;
    
    page->advance_cursor(from, to);
  };

  // advances to the next sequence step and returns all events to be played.
  //
  // this method is typically called by the sequence scheduler.
  std::vector<step_event_t> advance() {
    std::vector<step_event_t> next_events;
    
    // do not advance if we are not playing! just return empty vector!
    if (!transport->is_playing) return next_events;

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

  ////////////////////////////////////////
  //                                    //
  //    r e n d e r    m e t h o d s    //
  //                                    //
  ////////////////////////////////////////

  // renders this part from scratch
  void render() {
    page->render(get_cursor());
    ppqn->render();
    transport->render();
    render_last_step_ui(); // TODO encapsulate this somewhere??
  };

  // TODO where to put this???
  void render_last_step_ui() {
    if (page->follow_cursor) {
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
    
    if (page->steps->show_last) {
      monome_led_on(io->output.monome, config->mappings.last_step.x, config->mappings.last_step.y);
    } else if (!is_showing_last_step() && !page->follow_cursor) {
      monome_led_off(io->output.monome, config->mappings.last_step.x, config->mappings.last_step.y); 
    }
  }

  ////////////////////////////////////////
  //                                    //
  //    s e t t e r    m e t h o d s    //
  //                                    //
  ////////////////////////////////////////
  
  // add a step to the sequence. this adds the default midi note to the sequence.
  void add_step(unsigned int coarse_step_idx) {
    int abs_coarse_step = get_absolute_step(page->under_edit, coarse_step_idx);
    step_event_t event = midi_note_on(default_note, 0, 127);
    step_idx_t step = get_fine_step_index(abs_coarse_step);

    // TODO ....we should probably be using a mutex... -___-
    sequence.add_midi_note_on_event(event, step, 1);

    // add step to rendered steps
    page->steps->add(abs_coarse_step);
  };
  
  void remove_step(unsigned int coarse_step_idx) {
    int abs_coarse_step = get_absolute_step(page->under_edit, coarse_step_idx);
    step_idx_t step = get_fine_step_index(abs_coarse_step);
    sequence.remove_step(step);

    // remove step from rendered steps
    page->steps->remove(abs_coarse_step);
  };

  // sets the ppqn given a sequential index.
  //
  // if the part is currently playing, set the next ppqn to be scheduled
  // otherwise, immediately set the current ppqn.
  void set_ppqn(int idx) {
    if (transport->is_playing) {
      ppqn->set_next(idx);
    } else {
      ppqn->set(idx);
    }

    if (is_rendered) ppqn->render();
  };
  
  // updates the state of last step of a part and updates the ui (monome grid).
  //
  // this function assumes that the currently rendered page is the page on which
  // the new last step lies. this is important because the coordinates supplied for
  // the new last step are relative to the currently rendered page.
  void set_last_step(mapping_coordinates_t step_c) {
    step_idx_t last_step = get_absolute_step(page->rendered, config->mappings.steps.get_sequential_index_from_coordinates(step_c.x, step_c.y));

    page->steps->set_last_step(last_step);

    page->steps->render(page->rendered, get_cursor());
  };

  ////////////////////////////////////////////
  //                                        //
  //    p u b l i c    u t i l i t i e s    //
  //                                        //
  ////////////////////////////////////////////

  bool is_showing_last_step() {
    return page->steps->show_last;
  }

  step_idx_t get_cursor() {
    return active_step / constants::PPQN_MAX;
  }
  
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

  //////////////////////////////////////////////
  //                                          //
  //    p r i v a t e    u t i l i t i e s    //
  //                                          //
  //////////////////////////////////////////////

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

  bool is_step_visible(int coarse_step) {
    unsigned int page_size = config->mappings.steps.get_area();
    int min_visible_step = (page->rendered * page_size);
    int max_visible_step = ((page->rendered + 1) * page_size) - 1;
    return min_visible_step <= coarse_step && coarse_step <= max_visible_step;
  };
};


#endif
