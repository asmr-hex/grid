#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <queue>
#include <chrono>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <RtMidi.h>

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
  void run_dispatcher();
  void dispatch_event_loop();
  void dispatch();
  void dispatch_osc(step_event);
  void dispatch_midi(step_event);
  void enqueue_next_step();

  int tick_count;
  Microseconds tick_period;
  
  float bpm;
  float ppqn;

  std::queue <step_event> next_step_events;
  boost::thread dispatcher_thread;

  RtMidiOut *midiout;
};

#endif
