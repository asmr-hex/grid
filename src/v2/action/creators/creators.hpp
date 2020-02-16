#ifndef ACTION_CREATORS_H
#define ACTION_CREATORS_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/actions.hpp"

#include "anemone/state/root.hpp"
#include "anemone/action/types.hpp"


namespace action {

  // action::Creators is a collection of all action creator functions. each function
  // has access to IO in order to determine information about the grid and midi devices
  // when creating actions.
  //
  // for example, when advancing the tick of a part, we need to know the size of the
  // sequencer ui panel on the grid to determine page sizes, etc.
  class Creators {
  public:
    std::shared_ptr<IO> io;
    State::root_t state;
    std::shared_ptr<GridLayouts> layouts;

    Creators(std::shared_ptr<IO>, std::shared_ptr<GridLayouts>);

    step_cursor_updated advance_step(State::instrument_t);
    step_updated update_step(const types::step::page_relative_idx_t&);

    instrument_played_or_paused play_or_pause_rendered_instrument();
  };

}

#endif
