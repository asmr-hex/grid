#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <chrono>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::microseconds Microseconds;

class Sequencer {
public:
  Sequencer();
  void start();
  void stop();
  void pause();
  void schedule();
private:
  void dispatch();
  void run_dispatcher();

  int tick_count;
  Microseconds tick_period;
  
  float bpm;
  float ppqn;

  boost::thread dispatcher_thread;
};

#endif
