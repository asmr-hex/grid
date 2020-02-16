#include "anemone/state/instruments/state.hpp"

#include <spdlog/spdlog.h>


State::Instruments::Instruments() {
  spdlog::debug("    - constructing instruments");

  // create pointer to instrument by name map for closure below
  std::map<InstrumentName, instrument_t*> *by_name = new std::map<InstrumentName, instrument_t*>();

  // add instruments to by name map
  by_name->insert( { InstrumentName::ER1, er1.state->get() } );
  
  state = rx::State<instruments_t>
    ::with_actions<action_t>
    ::compose<er1_t*>
    ([by_name] (er1_t *_er1) -> instruments_t {
       return {
               .er1      = _er1,
               .rendered = InstrumentName::ER1, // TODO create rendered state for changing which instrument is rendered!
               .by_name  = by_name,
       };
     }, er1.state);
}

std::shared_ptr<rx::dag::Observable<State::instruments_t> > State::Instruments::get() {
  return state;
}
