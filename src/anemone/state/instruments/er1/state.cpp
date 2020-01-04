#include "anemone/state/instruments/er1/state.hpp"

#include <spdlog/spdlog.h>


State::ER1::ER1() : instrument(InstrumentName::ER1) {
  spdlog::debug("    - constructing er1");

  // create pointer to er1_t struct for closure below
  er1_t *er1 = new er1_t();
  
  state = rx::State<er1_t*>
    ::with_actions<action_t>
    ::compose<er1_settings_t, instrument_t>
    ([er1] (er1_settings_t settings, instrument_t instrument) -> er1_t* {
       er1->name     = instrument.name;
       er1->status   = instrument.status;
       er1->parts    = instrument.parts;
       er1->settings = settings;

       return er1;
     }, settings.state, instrument.state);

}

std::shared_ptr<rx::dag::Observable<State::er1_t*> > State::ER1::get() {
  return state;
}
