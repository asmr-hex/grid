#ifndef IO_GRID_LAYOUT_LAYOUTS_SEQUENCER_H
#define IO_GRID_LAYOUT_LAYOUTS_SEQUENCER_H

#include "anemone/config/config.hpp"
#include "anemone/io/grid/layout/layout.hpp"
#include "anemone/io/grid/layout/section.hpp"
#include "anemone/io/grid/layout/names.hpp"


namespace GridLayout {

  class Sequencer : public Layout {
  public:
    GridSection instrument_select;
    GridSection instrument_panel;
    GridSection steps;
    GridSection pages;
    GridSection parts;
    GridSection banks;
    GridSection ppqn;
    GridSection shift;
    GridSection play_pause;
    GridSection stop;
    GridSection record;
    GridSection last_step;
    GridSection metronome;

    Sequencer(std::shared_ptr<Config> config) : Layout(LayoutName::SequencerAndInstrument) {
      instrument_select = config->layouts.sequencer.instrument_select;
      instrument_panel = config->layouts.sequencer.instrument_panel;
      steps = config->layouts.sequencer.steps;
      pages = config->layouts.sequencer.pages;
      parts = config->layouts.sequencer.parts;
      banks = config->layouts.sequencer.banks;
      ppqn = config->layouts.sequencer.ppqn;
      shift = config->layouts.sequencer.shift;
      play_pause = config->layouts.sequencer.play_pause;
      stop = config->layouts.sequencer.stop;
      record = config->layouts.sequencer.record;
      last_step = config->layouts.sequencer.last_step;
      metronome = config->layouts.sequencer.metronome;

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
