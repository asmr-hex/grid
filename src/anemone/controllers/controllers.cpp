#include "anemone/controllers/controllers.hpp"


Controllers::Controllers(std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : io(io), state(state)
{}

void Controllers::connect() {
  layout = std::make_unique<LayoutController>(io, state);
  shift = std::make_unique<ShiftController>(io, state);
}
