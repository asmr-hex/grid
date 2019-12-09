#ifndef IO_GRID_OBSERVER_H
#define IO_GRID_OBSERVER_H

#include "types.hpp"


class GridObserver {
public:
  virtual void handle(grid_event_t) = 0;

  // NOTE: we don't need this if the only observer is the actions Dispatcher...
  // virtual bool is_relevant(grid_event_t) = 0;
};

#endif
