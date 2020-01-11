#ifndef STATE_TREE_ROOT_H
#define STATE_TREE_ROOT_H

#include <thread>
#include <memory>
#include <utility>

#include "anemone/util/concurrent_queue.hpp"

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/root.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"
#include "anemone/state/sequencer/state.hpp"
#include "anemone/state/instruments/state.hpp"
#include "anemone/state/high_frequency/step_cursors.hpp"


namespace State {
  
  // define composite root type
  struct root_t {
    sequencer_t sequencer;
    instruments_t instruments;
    std::shared_ptr<step_cursors_t> step_cursors;
    
    bool operator==(const root_t& rhs) {
      return
        sequencer   == rhs.sequencer &&
        instruments == rhs.instruments;
    };
  };

  class Root : public rx::Observable<root_t>, public rx::Root<action_t, high_freq_action_t> {
  public:
    rx::types::state_ptr<root_t, action_t> state;
    Sequencer   sequencer;
    Instruments instruments;

    // high frequenncy states
    StepCursors step_cursors;
    
    Root();
    virtual std::shared_ptr<rx::dag::Observable<root_t> > get() override;
    
    virtual void send_action(const action_t&) override;
    virtual void send_action(action_t&&) override;

    virtual void send_action(const high_freq_action_t&) override;
    virtual void send_action(high_freq_action_t&&) override;
    
    virtual void listen() override;
    
  private:
    std::unique_ptr<Queue<action_t> > queue;
  };
  
}

#endif
