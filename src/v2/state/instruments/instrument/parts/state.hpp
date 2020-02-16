#ifndef STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_H
#define STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/state/instruments/instrument/parts/part/state.hpp"


namespace State {

  // MIGHT NEED TO REDEFINE THE == operator for this typedef... (can we do this within the vector composition code?)
  typedef std::vector<part_t> *parts_t;
  
  class Parts : public rx::Observable<parts_t> {
  public:
    rx::types::state_ptr<parts_t, action_t> state;
    std::vector<Part> parts;

    Parts();

    virtual std::shared_ptr<rx::dag::Observable<parts_t> > get() override;
  };
}

#endif
