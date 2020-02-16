#include "anemone/state/selectors/steps.hpp"


bool is_step_activated(State::root_t state,
                       State::InstrumentName instrument,
                       types::part::idx_t part,
                       types::page::idx_t page,
                       types::step::page_relative_idx_t index)
{
  try {
    auto page_steps = get_rendered_steps(state, instrument, part).at(page);
    return page_steps.find(index) != page_steps.end();
  } catch (std::out_of_range &error) {
    // this part doesn't exist. so this step is not activated!
    return false;
  }
}


types::rendered_steps_t get_rendered_steps(State::root_t state,
                                           State::InstrumentName instrument,
                                           types::part::idx_t part)
{
  return state.sequences->at(instrument).at(part).rendered_steps;
}

