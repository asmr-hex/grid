#include "anemone/state/instruments/instrument/parts/part/state.hpp"

#include "anemone/util/match.hpp"

#include <spdlog/spdlog.h>


State::Part::Part(int id) {
  spdlog::debug("    - constructing part ({})", id);
  
  state = rx::State<part_t>::with_reducer<action_t>
    (part_t{ .id = id },
     [] (part_t old_state, action_t action) -> part_t {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
}

std::shared_ptr<rx::dag::Observable<State::part_t> > State::Part::get() {
  return state;
}
