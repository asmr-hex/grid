#ifndef CONFIG_LAYOUTS_SEQUENCER_H
#define CONFIG_LAYOUTS_SEQUENCER_H

#include "anemone/config/base.hpp"
#include "anemone/config/layouts/layout.hpp"
#include "anemone/state/layout/section.hpp"


namespace Conf {
  namespace Layout {

    class Sequencer : public LayoutBase {
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

      Sequencer(Base);
    };
    
  }
}

#endif
