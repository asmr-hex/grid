#include "anemone/controllers/step.hpp"


StepController::StepController(std::shared_ptr<IO>, std::shared_ptr<State>) {
  // subscribe to clock ticks
  auto tick_events = io->clock_events
    .subscribe([state] (tick_t t) {
                 state->instruments->advance_step();
               });
}
