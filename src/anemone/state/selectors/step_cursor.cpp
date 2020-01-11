#include "anemone/state/selectors/step_cursor.hpp"


State::step_cursor_t get_step_cursor_for(State::instrument_t instrument, int part_idx, State::root_t state) {
  return state.step_cursors->at(instrument.name)[part_idx];
}
