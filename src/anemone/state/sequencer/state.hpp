#ifndef STATE_TREE_SEQUENCER_H
#define STATE_TREE_SEQUENCER_H

#include <memory>

#include "anemone/util/match.hpp"

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"

#include "anemone/rx/dag/node.hpp"
#include "anemone/rx/dag/observable.hpp"


namespace State {

  struct sequencer_t {
    // TODO implement me
    int idk = 666;

    bool operator==(const sequencer_t& rhs) {
      return idk == rhs.idk;
    };
  };
  
  class Sequencer : public rx::Observable<sequencer_t> {
  public:
    rx::types::state_ptr<sequencer_t, action_t> state;
    
    Sequencer() {
      state = rx::State<sequencer_t>::with_reducer<action_t>
        (sequencer_t{},
         [] (sequencer_t old_state, action_t action) -> sequencer_t {
           return
             match(action,
                   [&] (const int& a) {
                     return sequencer_t{999};
                   },
                   [&] (const auto& a) {
                     return old_state;
                   });
         });      
    };

    virtual std::shared_ptr<rx::dag::Observable<sequencer_t> > get() override {
      return state;
    };
    
  };
    
}

#endif
