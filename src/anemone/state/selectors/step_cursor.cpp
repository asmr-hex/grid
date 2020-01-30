#include "anemone/state/selectors/step_cursor.hpp"
#include "anemone/state/selectors/parts.hpp"

#include <spdlog/spdlog.h>


State::step_cursor_t get_step_cursor_for(State::instrument_t instrument, types::part::idx_t part_idx, State::root_t state) {
  return state.step_cursors->at(instrument.name)[part_idx];
}

bool step_cursor_has_changed_for(State::instrument_t instrument, State::root_t state) {
  auto part = get_playing_part_id(instrument);
  auto step_cursor = get_step_cursor_for(instrument, part, state);

  auto previous = step_cursor.previous_page_relative_step;
  auto current = step_cursor.current_page_relative_step;
  if (previous.step != current.step) {
    return true;
  }

  return false;
}
