#include "anemone/state/instruments/instrument/parts/state.hpp"

#include "anemone/util/match.hpp"

#include <spdlog/spdlog.h>


State::Parts::Parts() {
  spdlog::debug("    - constructing parts");
  
  state = rx::State<parts_t>::with_reducer<action_t>
    (parts_t{},
     [] (parts_t old_state, action_t action) -> parts_t {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
}


std::shared_ptr<rx::dag::Observable<State::parts_t> > State::Parts::get() {
  return state;
}
