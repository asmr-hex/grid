#include "anemone/controllers/sequencer/play_pause.hpp"

#include <spdlog/spdlog.h>


ctrl::sequencer::PlayPause::PlayPause(std::shared_ptr<IO> io,
                              std::shared_ptr<GridLayouts> layouts,
                              std::shared_ptr<State::Root> state,
                              std::shared_ptr<Dispatcher> dispatcher,
                              std::shared_ptr<GridSection> grid_section)
  : Controller(io, layouts, state, dispatcher, grid_section) {}


void ctrl::sequencer::PlayPause::handle(const grid_event_t& event) {
  // TODO include last step setter (if shift is held)

  switch (event.type) {
  case GridEvent::Pressed:
    dispatch(make_action.play_or_pause_rendered_instrument());
    break;
  case GridEvent::Unpressed:
    break;
  }
}
