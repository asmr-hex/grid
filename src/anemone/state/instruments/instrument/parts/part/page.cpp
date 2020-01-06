#include "anemone/state/instruments/instrument/parts/part/page.hpp"

#include "anemone/utils/match.hpp"


State::Page::Page() {
  state = rx::State<page_t>
    ::with_reducers<action_t>
    (page_t{},
     [] (page_t old_state, action_t action) -> page_t {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
}


std::shared_ptr<rx::dag::Observable<State::page_t> > State::Page::get() {
  return state;
}
