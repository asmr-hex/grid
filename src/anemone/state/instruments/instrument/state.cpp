#include "anemone/state/instruments/instrument/state.hpp"

#include <spdlog/spdlog.h>


State::Instrument::Instrument(InstrumentName name) {
  spdlog::debug("    - constructing instrument ({})", name);
  
  state = rx::State<instrument_t>
    ::with_actions<action_t>
    ::compose<instrument_status_t, parts_t>
    ([name] (instrument_status_t _status, parts_t _parts) -> instrument_t {
       return {
               .name = name,
               .status = _status,
               .parts = _parts,
       };
     }, status.state, parts.state);
}

std::shared_ptr<rx::dag::Observable<State::instrument_t> > State::Instrument::get() {
  return state;
}
