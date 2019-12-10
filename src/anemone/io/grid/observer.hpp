#ifndef IO_GRID_OBSERVER_H
#define IO_GRID_OBSERVER_H

#include "types.hpp"


class GridObserver : public std::enable_shared_from_this<GridObserver> {
public:
  virtual void handle(grid_event_t) = 0;

  void subscribe(const std::shared_ptr<Grid>& grid) {
    grid->register_observer(shared_from_this());
  };
  // NOTE: we don't need this if the only observer is the actions Dispatcher...
  // virtual bool is_relevant(grid_event_t) = 0;
};

#endif
