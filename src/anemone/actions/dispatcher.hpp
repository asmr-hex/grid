#ifndef ACTIONS_DISPATCHER_H
#define ACTIONS_DISPATCHER_H

#include "../io/grid/observer.hpp"
#include "../io/midi/observer.hpp"


class Dispatcher : public GridObserver, public MidiObserver {
public:
  void handle(grid_event_t event) {
    // translate raw grid event into an action!
    // TODO defer to grid action generator class!
  };

  void handle(midi_event_t event) {
    // TODO translate this into an action, but preserve the original midi message i think...
  };

  void register_observer() {
    
  };
  
  // notify should dispatch actions to observers
  void broadcast(Action *action) {
    
  };
};

#endif
