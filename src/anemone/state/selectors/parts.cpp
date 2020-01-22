#include "anemone/state/selectors/parts.hpp"


State::part_t get_rendered_part(State::root_t state) {
  auto rendered_instrument = state.instruments.by_name->at(state.instruments.rendered);
  return rendered_instrument->parts->at(rendered_instrument->status.part.under_edit);
}


types::part::idx_t get_rendered_part_id(State::root_t state) {
  auto rendered_instrument = state.instruments.by_name->at(state.instruments.rendered);
  return rendered_instrument->status.part.under_edit;
}
