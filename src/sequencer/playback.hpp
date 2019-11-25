#ifndef SEQUENCER_PLAYBACK_H
#define SEQUENCER_PLAYBACK_H

#include <functional>

#include "../io/io.hpp"

#include "../animation/types.hpp"
#include "../animation/animator.hpp"

#include "../config/config.hpp"


class Playback {
public:
  // the part id and bank which this playback state belongs to. we
  // need this in order to hand off playback to a new part once a
  // previous part has completed its sequence cycle.
  struct {
    int id;
    int bank;
  } part;

  // playback state
  bool is_playing          = false;
  bool is_paused           = false;
  bool is_stopping         = false;
  bool is_about_to_start   = false;
  bool is_about_to_unpause = false;

  // the next playback state we wish to transition to. this is used
  // when we are handing over playback automatically from one part to another.
  Playback * next;

  Playback(int part_id,
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

  void render() {
    
  };

  void play() {
    
  };

  void pause() {
    
  };

  void stop() {
    
  };
  
private:
  IO *io;
  Config *config;
  Animator *animation;
  std::function<void (int, int)> swap_part_in_playback;
  
};

#endif
