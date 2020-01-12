#ifndef STATE_HIGH_FREQUENCY_STEP_CURSORS_H
#define STATE_HIGH_FREQUENCY_STEP_CURSORS_H

#include <map>
#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/types/sequencer.hpp"
#include "anemone/state/instruments/names.hpp"


namespace State {

  struct step_cursor_t {
    types::step::granular_idx_t current_step = 0;
    types::page::idx_t          current_page = 0;
  };
  

  typedef std::map<InstrumentName, std::map<unsigned int, step_cursor_t> > step_cursors_t;

  
  class StepCursors : public rx::Observable<std::shared_ptr<step_cursors_t> > {
  public:
    rx::types::high_frequency::state_ptr<step_cursors_t, high_freq_action_t> state;

    StepCursors();

    virtual std::shared_ptr<rx::dag::Observable<std::shared_ptr<step_cursors_t> > > get() override;
  };
}

#endif
