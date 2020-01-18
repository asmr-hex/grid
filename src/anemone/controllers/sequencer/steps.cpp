#include "anemone/controllers/sequencer/steps.hpp"

#include <spdlog/spdlog.h>


ctrl::sequencer::Steps::Steps(std::shared_ptr<IO> io,
                              std::shared_ptr<GridLayouts> layouts,
                              std::shared_ptr<State::Root> state,
                              std::shared_ptr<Dispatcher> dispatcher,
                              std::shared_ptr<GridSection> grid_section)
  : Controller(io, layouts, state, dispatcher, grid_section) {}


void ctrl::sequencer::Steps::handle(const grid_event_t& event) {
  spdlog::debug("Steps Section Grid Event Occurred!");
}
