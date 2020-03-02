/**
 * @file   state/instruments/part.hpp
 * @brief  Instrument Part Class
 * @author coco
 * @date   2020-02-20
 *************************************************/

#ifndef STATE_INSTRUMENTS_PART_H
#define STATE_INSTRUMENTS_PART_H

#include <memory>
#include <vector>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"

#include "anemone/state/instruments/sequence.hpp"


class Part {
public:
  Part(part_idx_t);

  part_idx_t id;
  // TODO inntroduce bank_relative_part_idx_t?

  struct Ppqn {
    rx::behavior<PPQN> current;
    rx::behavior<PPQN> next;
    rx::behavior<bool> pending_change;
  };

  struct Page {
    rx::behavior<page_idx_t> rendered;
    rx::behavior<page_idx_t> under_edit;
    rx::behavior<page_idx_t> last;
    rx::behavior<bool>       follow_cursor;
  };

  struct Transport {
    rx::behavior<bool> is_playing;
    rx::behavior<bool> is_paused;
    rx::behavior<bool> is_stopped;

    rx::behavior<bool> is_stopping;
    rx::behavior<bool> is_transitioning;
    rx::behavior<bool> is_about_to_start;
    rx::behavior<bool> is_about_to_unpause;

    // since pausing a part may leave its cursor not 'on the beat', we must
    // keep track of how much 'off the beat' it has been paused. this allows
    // us to resume a paused part appropriately so that, once resumed, it is
    // still aligned with played 'on the beat'.
    rx::behavior<int> pulse_pause_offset;
  };

  struct Step {
    rx::behavior<granular_step_idx_t> current;
    // rx::behavior<paged_step_idx_t>    current_page_relative = { .page = 0, .step = 0 }; // TODO remove this redundancy!???
    rx::behavior<paged_step_idx_t>    last;
    rx::behavior<bool>                show_last;

    /// @brief update the current step.
    void update_current(granular_step_idx_t);
  };
  
  Ppqn ppqn;
  Page page;
  Transport transport;
  Step step;
  Sequence sequence;
  bool unsaved;
};

#endif
