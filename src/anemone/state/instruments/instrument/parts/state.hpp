#ifndef STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_H
#define STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"


namespace State {

  struct parts_t {
    bool change_me = false;
    
    bool operator==(const parts_t& rhs) {
      return
        change_me == rhs.change_me;
    };
  };


  class Parts : public rx::Observable<parts_t> {
  public:
    rx::types::state_ptr<parts_t, action_t> state;

    Parts();

    virtual std::shared_ptr<rx::dag::Observable<parts_t> > get() override;
  };
}

#endif
