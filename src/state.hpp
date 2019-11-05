#ifndef STATE_H
#define STATE_H

#include <map>
#include <string>

#include <monome.h>

#include "state_handlers.hpp"
#include "controllers/grid.hpp"
#include "instruments/instrument.hpp"

using namespace std;

/*
  sequencer state. separating from sequencer class for better oganization.
 */
class State {
public:
  State(monome_t *monome, map<string, Instrument*> instruments) :
    instruments(instruments),
    monome(monome)
  {
    // configure buttons
    buttons.push_back(new ShiftButton(monome, 0, 7));
  };

  // update function for monome callback
  void update(monome_event_t *event) {
    // iterate over all buttons
    for (Button *button : buttons) {
      button->process_event(event, this);
    }

    // iterate over all instruments
    for (auto element : instruments) {
      element.second->update_state(event);
    }
  };

  // TODO overload update to accept midi callback input

  vector<Button*> buttons;
  map<string, Instrument*> instruments;

  // internal state
  bool shift_enabled = false;
private:
  monome_t *monome;
};

#endif
