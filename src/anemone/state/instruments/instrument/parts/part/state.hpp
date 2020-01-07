#ifndef STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_PART_H
#define STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_PART_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/state/instruments/instrument/parts/part/ppqn.hpp"
#include "anemone/state/instruments/instrument/parts/part/page.hpp"
#include "anemone/state/instruments/instrument/parts/part/transport.hpp"
#include "anemone/state/instruments/instrument/parts/part/step.hpp"


namespace State {

  struct part_t {
    int id;
    ppqn_t ppqn;
    page_t page;
    transport_t transport;
    step_t step;
    // sequence_t sequence;
    
    // ????
    // bool unsaved;


    bool operator==(const part_t& rhs) {
      return
        id        == rhs.id &&
        ppqn      == rhs.ppqn &&
        page      == rhs.page &&
        transport == rhs.transport &&
        step     == rhs.step;
    };
  };

  class Part : public rx::Observable<part_t> {
  public:
    rx::types::state_ptr<part_t, action_t> state;
    Ppqn      ppqn;
    Page      page;
    Transport transport;
    Step     step;

    Part(int);
    
    virtual std::shared_ptr<rx::dag::Observable<part_t> > get() override;
  };

}

#endif
