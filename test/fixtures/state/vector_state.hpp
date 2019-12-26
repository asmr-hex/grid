#ifndef TEST_FIXTURES_STATES_VECTOR_STATE_H
#define TEST_FIXTURES_STATES_VECTOR_STATE_H

#include <vector>

#include "anemone/util/match.hpp"
#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/node.hpp"

#include "fixtures/actions.hpp"


namespace fixture {
  namespace state {

    class VectorState : public rx::Observable< std::vector<int> > {
    public:
      rx::types::state_ptr<std::vector<int>, fixture::action_t> state;

      VectorState() {
        state = rx::State<std::vector<int> >::with_reducer<fixture::action_t>
          ({4, 5, 6},
           [] (std::vector<int> old_state, fixture::action_t action) -> std::vector<int> {
             return
               match(action,
                     [&] (const fixture::action::append& a) {
                       old_state.push_back(a.item);
                       return old_state;
                     },
                     [&] (const fixture::action::clear& a) {
                       old_state.clear();
                       return old_state;
                     },
                     [&] (const auto& a) {
                       return old_state;
                     });
           });
      }

      virtual std::shared_ptr<rx::dag::Observable<std::vector<int> > > get() override {
        return state;
      }
      
    };
    
  }
}

#endif
