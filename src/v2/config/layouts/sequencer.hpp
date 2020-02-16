#ifndef CONFIG_LAYOUTS_SEQUENCER_H
#define CONFIG_LAYOUTS_SEQUENCER_H

#include "anemone/config/base.hpp"
#include "anemone/io/grid/layout/section.hpp"


namespace Conf {
  namespace Layout {

    class Sequencer : public Base {
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

    private:
      // TODO (coco|12.29.2019) implement function to ensure there are no overlapping sections!
      
      grid_region_t parse_grid_region(std::string);
    };
    
  }
}

#endif
