#ifndef STATE_SELECTORS_STEP_CURSOR_H
#define STATE_SELECTORS_STEP_CURSOR_H

#include "anemone/state/root.hpp"
#include "anemone/types.hpp"


State::step_cursor_t get_step_cursor_for(State::instrument_t, types::part::idx_t, State::root_t);

#endif
