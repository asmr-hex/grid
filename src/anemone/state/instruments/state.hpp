#ifndef STATE_TREE_INSTRUMENTS_H
#define STATE_TREE_INSTRUMENTS_H

#include <map>
#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/state/instruments/er1/state.hpp"
#include "anemone/state/instruments/instrument/state.hpp"


namespace State {

  struct instruments_t {
    er1_t *er1;
    std::map<InstrumentName, instrument_t*> *by_name;

    bool operator==(const instruments_t& rhs) {
      return
        (*er1) == (*rhs.er1);
    };
  };

  class Instruments : public rx::Observable<instruments_t> {
  public:
    rx::types::state_ptr<instruments_t, action_t> state;
    ER1 er1;
    
    Instruments();
    virtual std::shared_ptr<rx::dag::Observable<instruments_t> > get() override;
  };
}

#endif
