#include "anemone/state/instruments/instrument/parts/part/steps.hpp"

#include "anemone/util/match.hpp"


State::Steps::Steps() {
  rendered = new std::map<types::page::idx_t, std::set<types::step::idx_t> >();
  
  state = rx::State<steps_t>
    ::with_reducer<action_t>
    (steps_t{ .rendered = rendered },
     [] (steps_t old_state, action_t action) -> steps_t {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
}


std::shared_ptr<rx::dag::Observable<State::steps_t> > State::Steps::get() {
  return state;
}
