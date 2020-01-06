#ifndef STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_PPQN_H
#define STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_PPQN_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/types/sequencer.hpp"


namespace State {

  struct ppqn_t {
    PPQN current = PPQN::Eight;
    PPQN next;
    bool pending_change = false;

    bool operator==(const ppqn_t& rhs) {
      return
        current        == rhs.current &&
        next           == rhs.next    &&
        pending_change == rhs.pending_change;
    };
  };

  class Ppqn : public rx::Observable<ppqn_t> {
  public:
    rx::types::state_ptr<ppqn_t, action_t> state;

    Ppqn();
    
    virtual std::shared_ptr<rx::dag::Observable<ppqn_t> > get() override;
  };

}

#endif
