#ifndef ANIMATION_ANIMATOR_H
#define ANIMATION_ANIMATOR_H

#include <monome.h>
#include <boost/thread.hpp>

#include "types.hpp"
#include "../sequencer/types.hpp"


class Animator {
public:
  // TODO make frame period configurable via constructor
  
  void run() {
    animation_thread = boost::thread(&Animator::animation_loop, this);
  };

  void add() {
    // TODO include animation types!
  };

  void remove() {
    // TODO include animation types!
  };
private:
  Microseconds frame_period;
  boost::thread animation_thread;
  // TODO define data structure for storing registered animations!

  void animation_loop() {
    while (true) {
      auto tick = Clock::now();

      // iterate through animations, compute them, broadcast to monome!
      
      auto tock = Clock::now();
      Microseconds remaining_usec = frame_period - std::chrono::duration_cast<Microseconds>(tock - tick);

      boost::this_thread::sleep(boost::posix_time::microseconds(remaining_usec.count()));
    }
  };
};

#endif
