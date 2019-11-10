#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <map>
#include <string>
#include <vector>

#include <boost/thread.hpp>

#include "types.hpp"
#include "../io/io.hpp"
#include "../state/state.hpp"
#include "../instruments/instrument.hpp"


class Scheduler {
public:
  Scheduler(IO *io, State *state, std::map<std::string, Instrument *> instruments) :
    io(io),
    state(state),
    instruments(instruments){};
  
  void run() {
    dispatcher_thread = boost::thread(&Scheduler::dispatch_event_loop, this);
    dispatcher_thread.join(); // TODO: do we need to block here?
  };
private:
  IO *io;
  State *state;
  std::map<std::string, Instrument *> instruments;
  boost::thread dispatcher_thread;

  std::vector<step_event_t> fetch_next_step_events() {
    std::vector<step_event_t> current_step_events;
    
    for (auto it : instruments) {
      std::string name = it.first;
      Instrument * instrument = it.second;
      Part* part = instrument->get_current_part();

      // determine whether this is the current instrument on display.
      bool instrument_is_displayed = name == state->sequencer.active_instrument ? true : false;
      
      std::vector<step_event_t> new_step_events = part->advance(instrument_is_displayed);
      current_step_events.insert(current_step_events.end(),
                                 new_step_events.begin(),
                                 new_step_events.end());
    }

    return current_step_events;  
  };

  void dispatch_osc(step_event_t event) {
    // parse command and coordinates from event uid
    osc_command_t command = (osc_command_t)(event.id >> 8);
    unsigned int x = event.id >> 4;
    unsigned int y = event.id & 0x0F;

    switch (command) {
    case led_on:
      monome_led_on(io->output.monome, x, y);
      break;
    case led_off:
      monome_led_off(io->output.monome, x, y);
      break;
    case led_brightness:
      monome_led_level_set(io->output.monome, x, y, (unsigned int)(event.data[0]));
      break;
    default:
      std::cout << "Unrecognized OSC command " << command << std::endl;
    }
  };

  void dispatch_midi(step_event_t event) {
    io->output.midi->sendMessage(&event.data);
  };

  
  void dispatch() {
    std::vector<step_event_t> current_step_events = fetch_next_step_events();
    
    for (step_event_t event : current_step_events) {
      switch (event.protocol) {
      case OSC:
        dispatch_osc(event);
        break;
      case MIDI:
        dispatch_midi(event);
        break;
      } 
    }
  };
  
  void dispatch_event_loop() {  
    while (true) {
      auto tick = Clock::now();

      if (state->sequencer.playback.is_playing) {
        // only dispatch if we are currently playing.
        dispatch();
      }
 
      auto tock = Clock::now();
      Microseconds remaining_usec = state->sequencer.step_period - std::chrono::duration_cast<Microseconds>(tock - tick);
    
      boost::this_thread::sleep(boost::posix_time::microseconds(remaining_usec.count()));
  }
  };
};

#endif
