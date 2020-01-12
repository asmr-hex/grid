#include "anemone/state/instruments/instrument/parts/part/step.hpp"

#include "anemone/util/match.hpp"

#include <spdlog/spdlog.h>


State::Step::Step() {
  rendered = new std::map<types::page::idx_t, std::set<types::step::idx_t> >();
  
  state = rx::State<step_t>
    ::with_reducer<action_t>
    (step_t{ .rendered = rendered },
     [] (step_t old_state, action_t action) -> step_t {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
}


std::shared_ptr<rx::dag::Observable<State::step_t> > State::Step::get() {
  return state;
}
