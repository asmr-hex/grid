#include "anemone/state/instruments/part.hpp"


Part::Part(part_idx_t id)
  : id(id),
    ppqn({ .current        = rx::behavior<PPQN>(PPQN::Four),
           .previous       = rx::behavior<PPQN>(PPQN::Four),
           .next           = rx::behavior<PPQN>(PPQN::Four),
           .pending_change = rx::behavior<bool>(false),
      }),
    page({ .rendered      = rx::behavior<page_idx_t>(0),
           .under_edit    = rx::behavior<page_idx_t>(0),
           .last          = rx::behavior<page_idx_t>(1), // TODO NOTE: this is duplicated state with step.last .... remove one of them. probably this one and make this derived inn the ui
           .follow_cursor = rx::behavior<bool>(false),
      }),
    transport({ .is_playing          = rx::behavior<bool>(true),
                .is_paused           = rx::behavior<bool>(false),
                .is_stopped          = rx::behavior<bool>(false),
                .is_stopping         = rx::behavior<bool>(false),
                .is_transitioning    = rx::behavior<bool>(false),
                .is_about_to_start   = rx::behavior<bool>(false),
                .is_about_to_unpause = rx::behavior<bool>(false),
                .pulse_pause_offset  = rx::behavior<int>(0),
      }),
    step({ .current   = rx::behavior<granular_step_idx_t>(0),
           .last      = rx::behavior<paged_step_idx_t>({ .page = 1, .step = 31 }),
           .show_last = rx::behavior<bool>(false),
      })
{}

void Part::Step::update_current(granular_step_idx_t step) {
  current.get_subscriber().on_next(step);
}
