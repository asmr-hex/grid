#ifndef STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_TRANSPORT_H
#define STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_TRANSPORT_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/types/sequencer.hpp"


namespace State {

  struct transport_t {
    bool is_playing          = false;
    bool is_paused           = false;
    bool is_stopped          = false;

    bool is_stopping         = false;
    bool is_transitioning    = false;
    bool is_about_to_start   = false;
    bool is_about_to_unpause = false;

    // since pausing a part may leave its cursor not 'on the beat', we must
    // keep track of how much 'off the beat' it has been paused. this allows
    // us to resume a paused part appropriately so that, once resumed, it is
    // still aligned with played 'on the beat'.
    int pulse_pause_offset   = 0;

    bool operator==(const transport_t& rhs) {
      return
        is_playing          == rhs.is_playing          &&
        is_paused           == rhs.is_paused           &&
        is_stopped          == rhs.is_stopped          &&
        is_stopping         == rhs.is_stopping         &&
        is_transitioning    == rhs.is_transitioning    &&
        is_about_to_start   == rhs.is_about_to_start   &&
        is_about_to_unpause == rhs.is_about_to_unpause &&
        pulse_pause_offset  == rhs.pulse_pause_offset;
    };
  };

  class Transport : public rx::Observable<transport_t> {
  public:
    rx::types::state_ptr<transport_t, action_t> state;

    Transport();
    
    virtual std::shared_ptr<rx::dag::Observable<transport_t> > get() override;
  };

}

#endif
