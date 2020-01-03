#include "anemone/controllers/clock/scheduler.hpp"

#include <iostream>


ctrl::clock::Scheduler::Scheduler(std::shared_ptr<State::Root> state,
                                  std::shared_ptr<Dispatcher> dispatcher,
                                  std::shared_ptr<Clock> clock)
  : Controller(state, dispatcher, clock) {}


void ctrl::clock::Scheduler::handle(const tick_t& tick) {
  // advance steps
  std::cout << "TICK: " << tick << "\n";
}

