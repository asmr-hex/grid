#include "anemone/state/instruments/instrument/parts/part/transport.hpp"

#include "anemone/util/match.hpp"


State::Transport::Transport() {
  state = rx::State<transport_t>
    ::with_reducer<action_t>
    (transport_t{},
     [] (transport_t old_state, action_t action) -> transport_t {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
}


std::shared_ptr<rx::dag::Observable<State::transport_t> > State::Transport::get() {
  return state;
}
