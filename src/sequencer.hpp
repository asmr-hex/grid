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

enum Protocol { MIDI, OSC };

struct step_event {
  Protocol protocol;  
};

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
  void enqueue_next_step();

  int tick_count;
  Microseconds tick_period;
  
  float bpm;
  float ppqn;

  std::queue <int> next_step_events;
  boost::thread dispatcher_thread;

  RtMidiOut *midiout;
};

#endif
