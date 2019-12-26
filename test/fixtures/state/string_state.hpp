#ifndef TEST_FIXTURES_STATES_STRING_STATE_H
#define TEST_FIXTURES_STATES_STRING_STATE_H

#include <cctype>
#include <string>
#include <algorithm>

#include "anemone/util/match.hpp"
#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/node.hpp"

#include "fixtures/actions.hpp"


namespace fixture {
  namespace state {

    class StringState : public rx::Observable<std::string> {
    public:
      rx::types::state_ptr<std::string, fixture::action_t> state;

      StringState() {

        state = rx::State<std::string>::with_reducer<fixture::action_t>
          ("SineWaVe",
           [] (std::string old_state, fixture::action_t action) -> std::string {
             return
               match(action,
                     [&] (const fixture::action::lowercase& a) {
                       auto new_state = old_state;
                       std::transform(new_state.begin(), new_state.end(), new_state.begin(),
                                      [](unsigned char c) -> unsigned char { return std::tolower(c); });
                       return new_state;
                     },
                     [&] (const fixture::action::change_str& a) {
                       return a.new_str;
                     },
                     [&] (const auto& a) {
                       return old_state;
                     });
           });
      }

      virtual std::shared_ptr<rx::dag::Observable<std::string> > get() override {
        return state;
      }
    };
    
  }
}

#endif
