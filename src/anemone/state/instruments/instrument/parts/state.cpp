#include "anemone/state/instruments/instrument/parts/state.hpp"

#include "anemone/util/match.hpp"

#include <spdlog/spdlog.h>


State::Parts::Parts() {
  spdlog::debug("    - constructing parts");

  int n_parts = 49;
  std::vector<rx::types::state_ptr<part_t, action_t> > part_states;
      
  for (int i = 0; i < n_parts; i++) {
    auto part = Part(i);
    parts.push_back(part);
    part_states.push_back(part.state);
  }
      
  state = rx::State<part_t>
    ::with_actions<action_t>
    ::compose_vector
    (part_states,
     [] (action_t action) -> std::vector<int> {
       return
         match(action,
               [] (const auto& a) -> std::vector<int> {
                 return {};
               });
     });
}


std::shared_ptr<rx::dag::Observable<State::parts_t> > State::Parts::get() {
  return state;
}
