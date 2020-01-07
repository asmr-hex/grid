#include "anemone/controllers/controllers.hpp"

#include <spdlog/spdlog.h>


Controllers::Controllers(std::shared_ptr<IO> io,
                         std::shared_ptr<Clock> clock,
                         std::shared_ptr<GridLayouts> layouts,
                         std::shared_ptr<State::Root> state,
                         std::shared_ptr<Dispatcher> dispatcher)
  : io(io), clock(clock), layouts(layouts), state(state), dispatcher(dispatcher) {

  scheduler = std::make_shared<ctrl::clock::Scheduler>
    (ctrl::clock::Scheduler(io, layouts, state, dispatcher, clock));
}

void Controllers::connect_to_state() {
  scheduler->listen();

  spdlog::info("  initializing state -> controllers");
}
