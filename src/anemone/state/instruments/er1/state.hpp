#ifndef STATE_TREE_INSTRUMENTS_ER1_H
#define STATE_TREE_INSTRUMENTS_ER1_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/state/instruments/names.hpp"
#include "anemone/state/instruments/instrument/state.hpp"
#include "anemone/state/instruments/er1/settings.hpp"



namespace State {

  struct er1_t : instrument_t {
    er1_settings_t settings;

    bool operator==(const er1_t& rhs) {
      return
        settings               == rhs.settings &&
        (instrument_t&)(*this) == (const instrument_t&)rhs;
    };
  };


  class ER1 : public rx::Observable<er1_t*> {
  public:
    rx::types::state_ptr<er1_t*, action_t> state;
    ER1Settings settings;
    Instrument instrument;
    
    ER1();
    virtual std::shared_ptr<rx::dag::Observable<er1_t*> > get() override;
  };
  
}

#endif
