#ifndef ACTION_TYPES_H
#define ACTION_TYPES_H

#include <variant>

#include "anemone/action/sequencer.hpp"

namespace action {
  // used to initialize state tree and all connected components
  // this should always catch the default reducer branch to return
  // the default state.
  struct hydrate_state {};
}

using action_t = std::variant<action::hydrate_state,
                              action::update_bpm>;

#endif
