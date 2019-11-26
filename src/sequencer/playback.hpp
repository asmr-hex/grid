#ifndef SEQUENCER_PLAYBACK_H
#define SEQUENCER_PLAYBACK_H

#include <functional>

#include "../io/io.hpp"

#include "../animation/types.hpp"
#include "../animation/animator.hpp"

#include "../config/config.hpp"


class Transport {
public:
  // the part id and bank which this playback state belongs to. we
  // need this in order to hand off playback to a new part once a
  // previous part has completed its sequence cycle.
  struct {
    int id;
    int bank;
  } part;

  // playback states
  bool is_playing          = false;
  bool is_paused           = false;
  bool is_stopped          = true;

  // intermediate states
  bool is_stopping         = false;
  bool is_transitioning    = false;
  bool is_about_to_start   = false;
  bool is_about_to_unpause = false;

  // since pausing a part may leave its cursor not 'on the beat', we must
  // keep track of how much 'off the beat' it has been paused. this allows
  // us to resume a paused part appropriately so that, once resumed, it is
  // still aligned with played 'on the beat'.
  int pause_pulse_offset = 0;
  
  // the next playback state we wish to transition to. this is used
  // when we are handing over playback automatically from one part to another.
  Transport * next;

  Transport(int part_id,
           int part_bank,
           IO *io,
           Animator *animation,
           Config *config,
           std::function<void (int, int)> swap_part_in_playback)
    : io(io),
      animation(animation),
      config(config),
      swap_part_in_playback(swap_part_in_playback) {
    part.id = part_id;
    part.bank = part_bank;
  };

  // renders the play/pause & stop buttons in the ui.
  //
  // upon rendering, all animations on play/pause and stop are
  // removed if we aren't paused or in an intermediate state.
  void render() {
    render_play_pause();
    render_stop();
  };

  // queues up a part for playback.
  // 
  // since playback starts 'on the beat', we don't start the part immediately
  // but wait until it is ready to play and set it into an intermediate state.
  void prepare_to_play() {
    is_playing = false;
    if (is_paused) is_about_to_unpause = true;
    if (is_stopped) is_about_to_start = true;
    is_stopping = false;
    is_transitioning = false;
  };

  // plays a part immediately.
  //
  // typically, this is called by a pulse handler within the Part::advance
  // method after we have called Transport::prepare_to_play.
  void play() {
    is_playing = true;
    is_paused = false;
    is_stopped = false;
    is_about_to_start = false;
    is_about_to_unpause = false;
    is_stopping = false;
    is_transitioning = false;
  }
  
  // pauses the current part.
  //
  // adds an animation for the pause button also.
  void pause(int pulse) {
    is_playing = false;
    is_paused = true;
    is_stopped = false;
    is_about_to_start = false;
    is_about_to_unpause = false;
    is_stopping = false;
    is_transitioning = false;
    
    pause_pulse_offset = pulse;
  };

  // stops the current part
  //
  // removes any animations which exist on the play/pause button.
  void stop() {
    is_playing = false;
    is_paused = false;
    is_stopped = true;
    is_about_to_start = false;
    is_about_to_unpause = false;
    is_stopping = false;
    is_transitioning = false;
  };

  // prepares this part to stop.
  //
  // after this method is called, typically the part is stopped within
  // the pulse handlers (sp. Part::on_cycle_updates).
  void prepare_to_stop() {
    is_stopping = true;
  }
  
  // sets up the next part to begin playing automatically.
  //
  // this method allows for smooth automatic transitions between the
  // currently playing part and a new track.
  void transition_to(Transport *next_transport) {
    prepare_to_stop();
    is_transitioning = true;
    
    if (playback.is_stopping) {
      // playback for this part is already stopping...
      // this means that a new part has been enqueued to start

      // set previously enqueued next part to *not* be about to play
      next->is_about_to_start = false;
    }
    
    next = next_transport;
    next->is_about_to_start = true;
  }

  // automatically hands over playback to a new part.
  //
  // this method is typically called from within the Part::advance
  // method when the current part's sequence cycle has ended. the next
  // part begins playing automatically.
  void handoff_playback() {
    // first, stop this part
    stop();

    // start next part
    next->play();

    // execute notify handler on instrument
    swap_part_in_playback(next->part.bank, next->part.id);
  }
  
private:
  IO *io;
  Config *config;
  Animator *animation;
  std::function<void (int, int)> swap_part_in_playback;

  // led brightness and animation state configurations
  struct {
    int is_playing = 15;
    int is_stopped = 0;
    waveform is_paused = {.amplitude.max = 8,
                          .amplitude.min = 4,
                          .modulator = { .type = Unit },
                          .pwm = { .duty_cycle = 0.5, .period = 500, .phase = 0}
    };
    waveform is_about_to_start = {.amplitude.max = 15,
                                  .amplitude.min = 0,
                                  .modulator = { .type = Unit },
                                  .pwm = { .duty_cycle = 0.2, .period = 300, .phase = 0}
    };
    waveform is_stopping = {.amplitude.max = 15,
                            .amplitude.min = 0,
                            .modulator = { .type = Unit },
                            .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0}
    };
  } led;

  // render the play_pause ui according to current state
  void render_play_pause() {
    int intensity;
    waveform w;

    if (is_playing) intensity = led.is_playing;
    if (is_stopped) intensity = led.is_stopped;
    if (is_about_to_start) w = led.is_about_to_start;
    if (is_paused) w = led.is_paused;

    // remove any pre-existing animation
    animation->remove(config->mappings.play_pause);

    // set animation or led
    bool is_animated = is_about_to_start || is_paused;
    if (is_animated)
      animation->add(w, config->mappings.play_pause);
    else
      monome_led_level_set(io->output.monome,
                           config->mappings.play_pause.x,
                           config->mappings.play_pause.y,
                           intensity);
  };

  // render the stop ui according to current state
  void render_stop() {
    int intensity;
    waveform w;

    if (is_stopped) intensity = led.is_stopped;
    if (is_stopping) w = led.is_stopping;

    // remove any pre-existing animation
    animation->remove(config->mappings.stop);

    // set animation or led
    bool is_animated = is_stopping;
    if (is_animated)
      animation->add(w, config->mappings.stop);
    else
      monome_led_level_set(io->output.monome,
                           config->mappings.stop.x,
                           config->mappings.stop.y,
                           intensity);
  };
};

#endif
