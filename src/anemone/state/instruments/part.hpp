/**
 * @file   state/instruments/part.hpp
 * @brief  Instrument Part Class
 * @author coco
 * @date   2020-02-20
 *************************************************/

#ifndef STATE_INSTRUMENTS_PART_H
#define STATE_INSTRUMENTS_PART_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"

#include "anemone/state/instruments/sequence.hpp"


class Part {
public:
  Part(part_idx_t);

  std::vector<step_event_t> advance_step();

  part_idx_t id;
  // TODO inntroduce bank_relative_part_idx_t?

  struct Ppqn {
    PPQN current        = PPQN::Four;
    PPQN next           = PPQN::Four;
    bool pending_change = false;
  };

  struct Page {
    rx::behavior<page_idx_t> rendered = rx::behavior<page_idx_t>(0);
    page_idx_t under_edit    = 0;
    page_idx_t last          = 0;
    bool       follow_cursor = false;
  };

  struct Transport {
    bool is_playing          = false;
    bool is_paused           = false;
    bool is_stopped          = false;

    bool is_stopping         = false;
    bool is_transitioning    = false;
    bool is_about_to_start   = false;
    bool is_about_to_unpause = false;

    // since pausing a part may leave its cursor not 'on the beat', we must
    // keep track of how much 'off the beat' it has been paused. this allows
    // us to resume a paused part appropriately so that, once resumed, it is
    // still aligned with played 'on the beat'.
    int pulse_pause_offset   = 0;  
  };

  struct Step {
    granular_step_idx_t current               = 0;
    paged_step_idx_t    current_page_relative = { .page = 0, .step = 0 };
    paged_step_idx_t    last                  = { .page = 1, .step = 31 };
    bool                show_last             = false;
  };
  
  Ppqn ppqn;
  Page page;
  Transport transport;
  Step step;
  Sequence sequence;
  bool unsaved;
};

#endif
