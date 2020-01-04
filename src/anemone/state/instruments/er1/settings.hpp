#ifndef STATE_TREE_INSTRUMENTS_ER1_SETTINGS_H
#define STATE_TREE_INSTRUMENTS_ER1_SETTINGS_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"


namespace State {

  struct er1_settings_t {
    bool change_me = false;
    
    bool operator==(const er1_settings_t& rhs) {
      return
        change_me == rhs.change_me;
    };
  };


  class ER1Settings : public rx::Observable<er1_settings_t> {
  public:
    rx::types::state_ptr<er1_settings_t, action_t> state;

    ER1Settings();

    virtual std::shared_ptr<rx::dag::Observable<er1_settings_t> > get() override;
  };
}

#endif
