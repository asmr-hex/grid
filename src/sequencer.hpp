#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

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
  float bpm;
  int ppqn;

  boost::thread dispatcher_thread;
};

#endif
