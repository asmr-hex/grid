#include "anemone/action/creators/creators.hpp"

#include "anemone/types/sequencer.hpp"
#include "anemone/state/instruments/names.hpp"

#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/layout/layouts/sequencer.hpp"



action::step_updated action::Creators::advance_step(State::instrument_t instrument) {
  // get currently playing part
  auto idx = instrument.status.part.in_playback;
  auto playing_part = instrument.parts->at(idx);

  // get page size
  unsigned int page_size = 0;
  // TODO refactor this to be easier!
  // TODO might need to refacto Layouts to avoid dynamic cast....! it is costly!
  if (io->grid->layout.name() == LayoutName::SequencerAndInstrument ) {
    auto sequencer_layout = std::dynamic_pointer_cast<GridLayout::Sequencer>
      (io->grid->layout.layout_by_name[LayoutName::SequencerAndInstrument]);

    page_size = sequencer_layout->steps.size();
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
