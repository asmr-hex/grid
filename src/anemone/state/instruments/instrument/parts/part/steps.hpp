#ifndef STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_STEPS_H
#define STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_STEPS_H

#include <map>
#include <set>
#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/types/sequencer.hpp"


namespace State {

  struct steps_t {
    types::step::granular_idx_t      current   = 0;
    types::step::page_relative_idx_t last      = { .page = 1, .step = 32 };
    bool                             show_last = false;

    std::map<types::page::idx_t, std::set<types::step::idx_t> > *rendered;

    bool operator==(const steps_t& rhs) {
      return
        current     == rhs.current   &&
        last        == rhs.last      &&
        show_last   == rhs.show_last &&
        (*rendered) == (*rhs.rendered);
    };
  };

  class Steps : public rx::Observable<steps_t> {
  public:
    rx::types::state_ptr<steps_t, action_t> state;

    Steps();
    
    virtual std::shared_ptr<rx::dag::Observable<steps_t> > get() override;

  private:
    std::map<types::page::idx_t, std::set<types::step::idx_t> > *rendered;
  };

}

#endif
