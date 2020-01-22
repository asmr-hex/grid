#ifndef STATE_SELECTORS_INSTRUMENTS_H
#define STATE_SELECTORS_INSTRUMENTS_H

#include "anemone/state/root.hpp"


std::vector<State::instrument_t> get_playing_instruments(State::root_t);

State::InstrumentName get_rendered_instrument_name(State::root_t);

#endif
