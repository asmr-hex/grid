#ifndef GRID_H
#define GRID_H

#include <monome.h>

// forward declaration to avoid circular dependency
class State;

class Button {
public:
  Button(monome_t *monome, unsigned int x, unsigned int y) : monome(monome) {
    x_range[0] = x;
    x_range[1] = x;
    y_range[0] = y;
    y_range[1] = y;
  };

  Button(monome_t *monome, unsigned int x_0, unsigned int x_1, unsigned int y_0, unsigned int y_1) :
    monome(monome)
  {
    x_range[0] = x_0;
    x_range[1] = x_1;
    y_range[0] = y_0;
    y_range[1] = y_1;
  };

  void process_event(monome_event_t *event, State *state) {
    // check if this button is within the valid range
    bool is_valid =
      event->grid.x >= x_range[0] &&
      event->grid.x <= x_range[1] &&
      event->grid.y >= y_range[0] &&
      event->grid.y <= y_range[1];
      
    if (!is_valid) return;

    // otherwise do some work!
    handle(event, state);
  }
protected:
  unsigned int x_range [2];
  unsigned int y_range [2];

  monome_t *monome;
  
  virtual void handle(monome_event_t *, State *) = 0;
};

#endif
