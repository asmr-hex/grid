#ifndef TEST_FIXTURES_STATES_INT_STATE_H
#define TEST_FIXTURES_STATES_INT_STATE_H

#include "anemone/util/match.hpp"
#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/node.hpp"

#include "fixtures/actions.hpp"


namespace fixture {
  namespace state {

    class IntState : public rx::Observable<int> {
    public:
      rx::types::state_ptr<int, fixture::action_t> state;

      IntState() {
        state = rx::State<int>::with_reducer<fixture::action_t>
          (666,
           [] (int old_state, fixture::action_t action) -> int {
             return
               match(action,
                     [&] (const fixture::action::increment& a) {
                       return old_state + 1;
                     },
                     [&] (const auto& a) {
                       return old_state; 
                     });
           });
      }

      virtual std::shared_ptr<rx::dag::Observable<int> > get() override {
        return state;
      }
    };
    
  }
}

#endif
