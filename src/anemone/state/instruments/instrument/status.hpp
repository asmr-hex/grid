#ifndef STATE_TREE_INSTRUMENTS_INSTRUMENT_STATUS_H
#define STATE_TREE_INSTRUMENTS_INSTRUMENT_STATUS_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"


namespace State {

  struct instrument_status_t {
    struct part_t {
      int in_playback = 0;
      int under_edit  = 0;

      bool operator==(const part_t& rhs) {
        return
          in_playback  == rhs.in_playback &&
          under_edit   == rhs.under_edit;
      };
    };

    struct bank_t {
      int in_playback = 0;
      int under_edit  = 0;

      bool operator==(const bank_t& rhs) {
        return
          in_playback  == rhs.in_playback &&
          under_edit   == rhs.under_edit;
      };
    };
    
    bool is_playing;
    bool stop_on_next_measure;
    part_t part;
    bank_t bank;

    bool operator==(const instrument_status_t& rhs) {
      return
        is_playing           == rhs.is_playing &&
        stop_on_next_measure == rhs.stop_on_next_measure &&
        part                 == rhs.part &&
        bank                 == rhs.bank;
    };
  };


  class InstrumentStatus : public rx::Observable<instrument_status_t> {
  public:
    rx::types::state_ptr<instrument_status_t, action_t> state;
    
    rx::types::state_ptr<bool, action_t> is_playing;
    rx::types::state_ptr<bool, action_t> stop_on_next_measure;
    rx::types::state_ptr<instrument_status_t::part_t, action_t> part;
    rx::types::state_ptr<instrument_status_t::bank_t, action_t> bank;

    InstrumentStatus();
    
    virtual std::shared_ptr<rx::dag::Observable<instrument_status_t> > get() override;
  };

}

#endif
