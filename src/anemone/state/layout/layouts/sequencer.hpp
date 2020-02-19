#ifndef IO_GRID_LAYOUT_LAYOUTS_SEQUENCER_H
#define IO_GRID_LAYOUT_LAYOUTS_SEQUENCER_H

#include <memory>

#include "anemone/config.hpp"
#include "anemone/state/layout/layout.hpp"
#include "anemone/state/layout/section.hpp"
#include "anemone/types/layout/names.hpp"


namespace GridLayout {

  // TODO make GridSections shared_ptrs!!!!!
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

    Sequencer(std::shared_ptr<Config> config) : Layout(LayoutName::SequencerAndInstrument) {
      instrument_select = std::make_shared<GridSection>(config->layouts.sequencer.instrument_select);
      instrument_panel  = std::make_shared<GridSection>(config->layouts.sequencer.instrument_panel);
      steps             = std::make_shared<GridSection>(config->layouts.sequencer.steps);
      pages             = std::make_shared<GridSection>(config->layouts.sequencer.pages);
      parts             = std::make_shared<GridSection>(config->layouts.sequencer.parts);
      banks             = std::make_shared<GridSection>(config->layouts.sequencer.banks);
      ppqn              = std::make_shared<GridSection>(config->layouts.sequencer.ppqn);
      shift             = std::make_shared<GridSection>(config->layouts.sequencer.shift);
      play_pause        = std::make_shared<GridSection>(config->layouts.sequencer.play_pause);
      stop              = std::make_shared<GridSection>(config->layouts.sequencer.stop);
      record            = std::make_shared<GridSection>(config->layouts.sequencer.record);
      last_step         = std::make_shared<GridSection>(config->layouts.sequencer.last_step);
      metronome         = std::make_shared<GridSection>(config->layouts.sequencer.metronome);

      register_section(instrument_select);
      register_section(instrument_panel);
      register_section(steps);
      register_section(pages);
      register_section(parts);
      register_section(banks);
      register_section(ppqn);
      register_section(shift);
      register_section(play_pause);
      register_section(stop);
      register_section(record);
      register_section(last_step);
      register_section(metronome);
    };
  };

}

#endif
