#ifndef STATE_SELECTORS_STEPS_H
#define STATE_SELECTORS_STEPS_H

#include "anemone/state/root.hpp"
#include "anemone/types.hpp"


bool is_step_activated(State::root_t,
                       State::InstrumentName,
                       types::part::idx_t,
                       types::page::idx_t,
                       types::step::page_relative_idx_t);

types::rendered_steps_t get_rendered_steps(State::root_t state,
                                           State::InstrumentName instrument,
                                           types::part::idx_t part);

#endif
