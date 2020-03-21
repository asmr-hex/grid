#ifndef IO_GRID_LAYOUT_LAYOUTS_SEQUENCER_H
#define IO_GRID_LAYOUT_LAYOUTS_SEQUENCER_H

#include <memory>

#include "anemone/config.hpp"

#include "anemone/types/io/grid/layout/names.hpp"
#include "anemone/types/io/grid/layout/layout.hpp"
#include "anemone/types/io/grid/layout/section.hpp"


namespace GridLayout {

  class Sequencer : public Layout {
  public:
    std::shared_ptr<GridSection> instrument_select;
    std::shared_ptr<GridSection> instrument_panel;
    std::shared_ptr<GridSection> steps;
    std::shared_ptr<GridSection> pages;
    std::shared_ptr<GridSection> parts;
    std::shared_ptr<GridSection> banks;
    std::shared_ptr<GridSection> ppqn;
    std::shared_ptr<GridSection> shift;
    std::shared_ptr<GridSection> play_pause;
    std::shared_ptr<GridSection> stop;
    std::shared_ptr<GridSection> record;
    std::shared_ptr<GridSection> last_step;
    std::shared_ptr<GridSection> metronome;

    // TODO make Instruments Sub-Layout and Override `translate` function
    // if the original translate function spits out an `InstrumentPanel` event,
    // re-translate with the Instrument Sub-Layout and return!
    // this mean moving all logic from, for example, state/instruments/er1/layout into the
    // state/layout/layouts/ dir for each instrument!
    // though.. there needs to be a way that this layout can get updates for which instrument is
    // currently rendered.....hmmmmmm
    
    Sequencer(std::shared_ptr<Config>);
  };
};

#endif
