#ifndef STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_PART_H
#define STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_PART_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"


namespace State {

  struct part_t {
    int id;
    ppqn_t ppqn;
    page_t page;
    transport_t transport;
    steps_t steps; // include last_step, current_step, rendered_steps,
    sequence_t sequence;
    
    // ????
    bool unsaved;


    bool operator==(const part_t& rhs) {
      return
        id        == rhs.id &&
        change_me == rhs.change_me;
    };
  };

  class Part : public rx::Observable<part_t> {
  public:
    rx::types::state_ptr<part_t, action_t> state;

    Part(int);
    
    virtual std::shared_ptr<rx::dag::Observable<part_t> > get() override;
  };

}

#endif
