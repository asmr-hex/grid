#include "anemone/state/instruments/instrument/parts/part/state.hpp"

#include "anemone/util/match.hpp"

#include <spdlog/spdlog.h>


State::Part::Part(int id) {
  spdlog::debug("    - constructing part ({})", id);
  
  state = rx::State<part_t>
    ::with_actions<action_t>
    ::compose<ppqn_t, page_t, transport_t, steps_t>
    ([id] (ppqn_t ppqn, page_t page, transport_t transport, steps_t steps) -> part_t {
       return {
               .id        = id,
               .ppqn      = ppqn,
               .page      = page,
               .transport = transport,
               .steps     = steps,
       };
     }, ppqn.state, page.state, transport.state, steps.state);
}

std::shared_ptr<rx::dag::Observable<State::part_t> > State::Part::get() {
  return state;
}
