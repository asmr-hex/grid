#include <spdlog/spdlog.h>

#include "anemone/controllers/controllers.hpp"


Controllers::Controllers(std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : io(io), state(state)
{}

void Controllers::connect() {
  // layout = std::make_unique<LayoutController>(io, state);
  shift = std::make_unique<ShiftController>(io, state);
  step = std::make_unique<StepController>(io, state);
  play_pause = std::make_unique<PlayPauseController>(io, state);
  // part = std::make_unique<PartController>(io, state);

  spdlog::info("  connected -> controllers");
}
