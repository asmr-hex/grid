#ifndef TEST_FIXTURES_STATES_STRUCT_STATE_H
#define TEST_FIXTURES_STATES_STRUCT_STATE_H

#include "anemone/util/match.hpp"
#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/node.hpp"

#include "fixtures/actions.hpp"


namespace fixture {
  namespace state {

    struct struct_state_t {
      bool on = false;
      int number = 888;

      bool operator==(const struct_state_t& rhs) const {
        return
          on     == rhs.on &&
          number == rhs.number;
      };
    };
    
    class StructState : public rx::Observable<struct_state_t> {
    public:
      rx::types::state_ptr<struct_state_t, fixture::action_t> state;

      StructState() {
        state = rx::State<struct_state_t>::with_reducer<fixture::action_t>
          (struct_state_t{},
           [] (struct_state_t old_state, fixture::action_t action) -> struct_state_t {
             return
               match(action,
                     [&] (const fixture::action::increment& a) {
                       auto new_state = old_state;
                       new_state.number++;
                       return new_state;
                     },
                     [&] (const fixture::action::toggle& a) {
                       auto new_state = old_state;
                       new_state.on = !new_state.on;
                       return new_state;
                     },
                     [&] (const auto& a) {
                       return old_state; 
                     });
           });
      }

      virtual std::shared_ptr<rx::dag::Observable<struct_state_t> > get() override {
        return state;
      };
    };
    
  }
}

#endif
