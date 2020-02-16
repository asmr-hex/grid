#include "anemone/state/selectors/instruments.hpp"


std::vector<State::instrument_t> get_playing_instruments(State::root_t state) {
  std::vector<State::instrument_t> playing_instruments;

  for ( auto itr : *state.instruments.by_name) {
    auto instrument = *(itr.second);
    if (instrument.status.is_playing)
      playing_instruments.push_back(instrument);             
  }

  return playing_instruments;
}


State::InstrumentName get_rendered_instrument_name(State::root_t state) {
  return state.instruments.rendered;
}
