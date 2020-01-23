#ifndef ACTION_STEPS_H
#define ACTION_STEPS_H

#include "anemone/state/instruments/names.hpp"
#include "anemone/types/sequencer.hpp"


namespace action {

  struct step_updated {
    State::InstrumentName    instrument_name;
    types::part::idx_t       part;
    types::step::idx_t       step;
    types::step::paged_idx_t page_relative;
  };

  struct step_activated {
    State::InstrumentName    instrument_name;
    types::part::idx_t       part;
    types::step::idx_t       step;
    types::step::paged_idx_t paged_step;
    types::step::event_t     event;
  };
}

#endif
