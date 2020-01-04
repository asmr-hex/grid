#include "anemone/state/instruments/er1/settings.hpp"

#include "anemone/util/match.hpp"

#include <spdlog/spdlog.h>


State::ER1Settings::ER1Settings() {
  spdlog::debug("    - constructing er1 settings");
  
  state = rx::State<State::er1_settings_t>::with_reducer<action_t>
    (State::er1_settings_t{},
     [] (State::er1_settings_t old_state, action_t action) -> State::er1_settings_t {
       return
         match(action,
               [&] (const auto& a) {
                 return old_state;
               });
     });
}


std::shared_ptr<rx::dag::Observable<State::er1_settings_t> > State::ER1Settings::get() {
  return state;
}
