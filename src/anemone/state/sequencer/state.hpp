#ifndef STATE_TREE_SEQUENCER_H
#define STATE_TREE_SEQUENCER_H

#include <memory>

#include "anemone/util/match.hpp"

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"

#include "anemone/rx/dag/node.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include <spdlog/spdlog.h>


namespace State {

  struct sequencer_t {
    float bpm = 120.0;

    bool operator==(const sequencer_t& rhs) {
      return bpm == rhs.bpm;
    };
  };
  
  class Sequencer : public rx::Observable<sequencer_t> {
  public:
    rx::types::state_ptr<sequencer_t, action_t> state;
    
    Sequencer() {
      spdlog::debug("    - constructing sequencer");

      state = rx::State<sequencer_t>::with_reducer<action_t>
        (sequencer_t{},
         [] (sequencer_t old_state, action_t action) -> sequencer_t {
           return
             match(action,
                   [&old_state] (const action::update_bpm& a) {
                     old_state.bpm = a.bpm;
                     return old_state;
                   },
                   [&old_state] (const auto& a) {
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
