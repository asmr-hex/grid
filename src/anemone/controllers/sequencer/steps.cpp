#include "anemone/controllers/sequencer/steps.hpp"

#include <spdlog/spdlog.h>


ctrl::sequencer::Steps::Steps(std::shared_ptr<IO> io,
                              std::shared_ptr<GridLayouts> layouts,
                              std::shared_ptr<State::Root> state,
                              std::shared_ptr<Dispatcher> dispatcher,
                              std::shared_ptr<GridSection> grid_section)
  : Controller(io, layouts, state, dispatcher, grid_section) {}


void ctrl::sequencer::Steps::handle(const grid_event_t& event) {
  // TODO include last step setter (if shift is held)

  // add a new step to the sequence using the most recent midi note
  switch (event.type) {
  case GridEvent::Pressed:
    dispatch(make_action.activate_step(event.index));
    break;
  case GridEvent::Unpressed:
    break;
  }
}
