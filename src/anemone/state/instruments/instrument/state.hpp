#ifndef STATE_TREE_INSTRUMENTS_INSTRUMENT_H
#define STATE_TREE_INSTRUMENTS_INSTRUMENT_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/state/instruments/names.hpp"
#include "anemone/state/instruments/instrument/status.hpp"
#include "anemone/state/instruments/instrument/parts/state.hpp"


namespace State {

  struct instrument_t {
    InstrumentName name;
    instrument_status_t status;
    parts_t parts;

    bool operator==(const instrument_t& rhs) {
      return
        name   == rhs.name   &&
        status == rhs.status &&
        parts  == rhs.parts;
    };
  };


  class Instrument : public rx::Observable<instrument_t> {
  public:
    rx::types::state_ptr<instrument_t, action_t> state;
    InstrumentStatus status;
    Parts parts;

    Instrument(InstrumentName);

    virtual std::shared_ptr<rx::dag::Observable<instrument_t> > get() override;
  };
}

#endif
