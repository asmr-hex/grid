#ifndef STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_PAGE_H
#define STATE_TREE_INSTRUMENTS_INSTRUMENT_PARTS_PAGE_H

#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/types/sequencer.hpp"


namespace State {

  struct page_t {
    types::page::idx_t rendered      = 0;
    types::page::idx_t under_edit    = 0;
    types::page::idx_t in_playback   = 0;
    types::page::idx_t last          = 1;
    bool               follow_cursor = false;

    bool operator==(const page_t& rhs) {
      return
        rendered      == rhs.rendered    &&
        under_edit    == rhs.under_edit  &&
        in_playback   == rhs.in_playback &&
        last          == rhs.last        &&
        follow_cursor == rhs.follow_cursor;
    };
  };

  class Page : public rx::Observable<page_t> {
  public:
    rx::types::state_ptr<page_t, action_t> state;

    Page();
    
    virtual std::shared_ptr<rx::dag::Observable<page_t> > get() override;
  };

}

#endif
