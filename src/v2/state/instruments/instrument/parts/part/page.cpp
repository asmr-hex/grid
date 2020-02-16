#include "anemone/state/instruments/instrument/parts/part/page.hpp"

#include "anemone/util/match.hpp"

#include <spdlog/spdlog.h>


State::Page::Page() {
  state = rx::State<page_t>
    ::with_reducer<action_t>
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
