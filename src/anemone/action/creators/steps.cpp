#include "anemone/action/creators/creators.hpp"

#include "anemone/types/sequencer.hpp"
#include "anemone/state/instruments/names.hpp"

#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/layout/layouts/sequencer.hpp"

#include "anemone/state/selectors/step_cursor.hpp"
#include "anemone/state/selectors/instruments.hpp"
#include "anemone/state/selectors/parts.hpp"
#include "anemone/state/selectors/pages.hpp"

#include <spdlog/spdlog.h>


action::step_updated action::Creators::advance_step(State::instrument_t instrument) {  
  // get currently playing part
  auto part_idx = instrument.status.part.in_playback;
  
  auto playing_part = instrument.parts->at(part_idx);

  // get page size
  unsigned int page_size = 0;
  // TODO refactor this to be easier?
  if (io->grid->layout.name() == LayoutName::SequencerAndInstrument ) {
    page_size = layouts->sequencer->steps->size();
  }
  
  // get current ppqn
  auto ppqn = static_cast<unsigned int>(playing_part.ppqn.current);

  // get step cursor for current innstrument/part
  auto step_cursor = get_step_cursor_for(instrument, part_idx, state);
  
  // get current & last step
  auto step = step_cursor.current_step;
  auto last_step = playing_part.step.last.to_absolute_idx(page_size);

  // increment step (by ppqn)
  step += ppqn;

  // cycle step if we are at the end of the sequence
  if ( step > (last_step * static_cast<unsigned int>(types::PPQN::Max)) )
    step = 0;

  auto sequence_relative_step = (step / static_cast<unsigned int>(types::PPQN::Max));
  
  // calculate the current page in playback
  types::step::paged_idx_t paged_idx = { .step = sequence_relative_step % page_size,
                                         .page = sequence_relative_step / page_size,
                                                        
  };

  return {
          .instrument_name = instrument.name,
          .part            = part_idx,
          .step            = step,
          .page_relative   = paged_idx,
  };
};

action::step_activated action::Creators::activate_step(const types::step::page_relative_idx_t& index) {
  State::InstrumentName name    = get_rendered_instrument_name(state);
  types::part::idx_t part       = get_rendered_part_id(state);
  types::step::paged_idx_t step = {
                                   .page = get_page_under_edit(state),
                                   .step = index,
  };

  return {
          .instrument_name = name,
          .part            = part,
          .step            = step,
  };
}
