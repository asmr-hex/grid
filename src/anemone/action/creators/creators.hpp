#ifndef ACTION_CREATORS_H
#define ACTION_CREATORS_H

#include <memory>

#include "anemone/io/io.hpp"
#include "anemone/state/root.hpp"
#include "anemone/action/types.hpp"
#include "anemone/io/grid/layout/layouts/layouts.hpp"


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
    std::shared_ptr<GridLayouts> layouts;

    Creators(std::shared_ptr<IO>, std::shared_ptr<GridLayouts>);

    step_updated advance_step(State::instrument_t);
    
  };

}

#endif
