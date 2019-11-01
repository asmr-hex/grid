#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <queue>
#include <map>
#include <string>
#include <chrono>
#include <iostream>

#include <monome.h>
#include <RtMidi.h>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include "config.hpp"
#include "instruments/instrument.hpp"
// #include "controllers/controller.hpp"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::microseconds Microseconds;

class Sequencer {
public:
  Sequencer(std::string);
  void start();
  void stop();
  void pause();
  void schedule();
private:
  std::string config_path;
  sequencer_config_t config;

  monome_t *monome;
  
  int ppqn;  // pulse per quarter note - global granularity
  float bpm; // beats per minute

  boost::thread dispatcher_thread;
  
  void configure();
  void initialize_instruments();
  void connect_io();
  
  void connect_to_midi_out();
  void run_dispatcher();
  void dispatch_event_loop();
  // void dispatch();
  // void dispatch_osc(step_event);
  // void dispatch_midi(step_event);
  // void enqueue_next_step();

  std::map<std::string, Instrument*> instruments;
  // std::map<Controller> controllers;
  
  int tick_count;
  Microseconds tick_period;


  

  // std::queue <step_event> next_step_events;
  

  RtMidiOut *midiout;
};

#endif
