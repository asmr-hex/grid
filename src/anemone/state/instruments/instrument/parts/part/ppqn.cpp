#include "anemone/state/instruments/instrument/parts/part/ppqn.hpp"

#include "anemone/utils/match.hpp"


State::Ppqn::Ppqn() {
  state = rx::State<ppqn_t>
    ::with_reducers<action_t>
    (ppqn_t{},
     [] (ppqn_t old_state, action_t action) -> ppqn_t {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
}


std::shared_ptr<rx::dag::Observable<State::ppqn_t> > State::Ppqn::get() {
  return state;
}
