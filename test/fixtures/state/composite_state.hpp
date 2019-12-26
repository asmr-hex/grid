#ifndef TEST_FIXTURES_STATES_COMPOSITE_STATE_H
#define TEST_FIXTURES_STATES_COMPOSITE_STATE_H

#include <string>
#include <vector>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/node.hpp"

#include "fixtures/state/vector_state.hpp"
#include "fixtures/state/struct_state.hpp"
#include "fixtures/actions.hpp"


namespace fixture {
  namespace state {

    struct composite_state_t {
      std::vector<int> vector;
      struct_state_t parameter;

      bool operator==(const composite_state_t& rhs) {
        return
          vector  == rhs.vector &&
          parameter == rhs.parameter;
      };
    };

    class CompositeState : public rx::Observable<composite_state_t> {
    public:
      rx::types::state_ptr<composite_state_t, fixture::action_t> state;
      VectorState vector_substate;
      StructState struct_substate;

      CompositeState() {
        vector_substate = VectorState();
        struct_substate = StructState();
        
        state = rx::State<composite_state_t>
          ::with_actions<fixture::action_t>
          ::compose<std::vector<int>, struct_state_t>
          ([] (std::vector<int> s, struct_state_t s_s) -> composite_state_t {
             return {
                     .vector  = s,
                     .parameter = s_s,
             };
           }, vector_substate.state, struct_substate.state);
      }

      virtual std::shared_ptr<rx::dag::Observable<composite_state_t> > get() override {
        return state;
      }
    };
    
  }
}

#endif
