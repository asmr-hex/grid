#include "anemone/state/instruments/instrument/status.hpp"

#include "anemone/util/match.hpp"

#include <spdlog/spdlog.h>


State::InstrumentStatus::InstrumentStatus() {
  spdlog::debug("    - constructing instrument status");

  
  is_playing = rx::State<bool>::with_reducer<action_t>
    (true,
     [] (bool old_state, action_t action) -> bool {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
  
  stop_on_next_measure = rx::State<bool>::with_reducer<action_t>
    (false,
     [] (bool old_state, action_t action) -> bool {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
  
  part = rx::State<instrument_status_t::part_t>::with_reducer<action_t>
    (instrument_status_t::part_t{},
     [] (instrument_status_t::part_t old_state, action_t action) -> instrument_status_t::part_t {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
  
  bank = rx::State<instrument_status_t::bank_t>::with_reducer<action_t>
    (instrument_status_t::bank_t{},
     [] (instrument_status_t::bank_t old_state, action_t action) -> instrument_status_t::bank_t {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
  
  state = rx::State<instrument_status_t>
    ::with_actions<action_t>
    ::compose<bool, bool, instrument_status_t::part_t, instrument_status_t::bank_t>
    ([] (bool is_playing,
         bool stop_on_next_measure,
         instrument_status_t::part_t part,
         instrument_status_t::bank_t bank) -> instrument_status_t {
       return {
               .is_playing           = is_playing,
               .stop_on_next_measure = stop_on_next_measure,
               .part                 = part,
               .bank                 = bank,
       };
     }, is_playing, stop_on_next_measure, part, bank); 
}

std::shared_ptr<rx::dag::Observable<State::instrument_status_t> > State::InstrumentStatus::get() {
  return state;
}
