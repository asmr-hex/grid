#include "anemone/action/creators/creators.hpp"

#include "anemone/types/sequencer.hpp"
#include "anemone/state/instruments/names.hpp"

#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/layout/layouts/sequencer.hpp"

#include <spdlog/spdlog.h>


action::step_updated action::Creators::advance_step(State::instrument_t instrument) {
  spdlog::debug("dispatching advance step action");
  
  // get currently playing part
  auto idx = instrument.status.part.in_playback;

  spdlog::debug("getting part at {}", idx);
  
  auto playing_part = instrument.parts->at(idx);

  // get page size
  unsigned int page_size = 0;
  // TODO refactor this to be easier?
  if (io->grid->layout.name() == LayoutName::SequencerAndInstrument ) {
    page_size = layouts->sequencer->steps.size();
  }
  
  // get current ppqn
  auto ppqn = playing_part.ppqn.current;
  
  // get current & last step
  auto step = playing_part.step.current;
  auto last_step = playing_part.step.last.to_absolute_idx(page_size);

  step += static_cast<unsigned int>(ppqn);

  if ( step > ((last_step * static_cast<unsigned int>(PPQN::Max)) - 1) )
    step = 0;
  
  return {
          .instrument_name = instrument.name,
          .step            = step,
  };
};
