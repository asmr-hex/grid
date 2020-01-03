#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include <memory>

#include "anemone/io/io.hpp"
#include "anemone/clock/clock.hpp"
#include "anemone/io/grid/layout/layouts/layouts.hpp"
#include "anemone/state/root.hpp"
#include "anemone/action/dispatcher.hpp"

#include "anemone/controllers/clock/scheduler.hpp"


class Controllers {
public:
  Controllers(std::shared_ptr<IO>,
              std::shared_ptr<Clock>,
              std::shared_ptr<GridLayouts>,
              std::shared_ptr<State::Root>,
              std::shared_ptr<Dispatcher>);

  void connect_to_state();
  
protected:
  std::shared_ptr<IO> io;
  std::shared_ptr<Clock> clock;
  std::shared_ptr<GridLayouts> layouts;
  std::shared_ptr<State::Root> state;
  std::shared_ptr<Dispatcher> dispatcher;

  std::shared_ptr<ctrl::clock::Scheduler> scheduler;
};

#endif
