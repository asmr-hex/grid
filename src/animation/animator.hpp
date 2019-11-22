#ifndef ANIMATION_ANIMATOR_H
#define ANIMATION_ANIMATOR_H

#include <map>

#include <monome.h>
#include <boost/thread.hpp>

#include "types.hpp"
#include "../io/io.hpp"
#include "../sequencer/types.hpp"
#include "../config/mappings/types.hpp"


class Animator {
public:
  // TODO make frame period configurable via constructor
  Animator(IO *io, Microseconds frame_period) : io(io), frame_period(frame_period) {};
  
  void run() {
    animation_thread = boost::thread(&Animator::animation_loop, this);
  };

  void add(waveform w, mapping_coordinates_t c) {
    animations[c] = w;
  };

  void remove(mapping_coordinates_t c) {
    animations.erase(c);
  };
  
private:
  IO *io;
  Microseconds frame_period;
  boost::thread animation_thread;
  std::map<mapping_coordinates_t, waveform> animations;
  unsigned int t = 0;
  
  void animation_loop() {
    while (true) {
      auto tick = Clock::now();
      
      // iterate through animations, compute them, broadcast to monome!
      for (auto i : animations) {
        int intensity = i.second.compute(t);
        monome_led_level_set(io->output.monome,
                             i.first.x,
                             i.first.y,
                             intensity);
      }

      t++;
      
      auto tock = Clock::now();
      Microseconds remaining_usec = frame_period - std::chrono::duration_cast<Microseconds>(tock - tick);

      boost::this_thread::sleep(boost::posix_time::microseconds(remaining_usec.count()));
    }
  };
};

#endif
