/**
 * @file   state/instruments/er1/layout.hpp
 * @brief  ER1 Instrument Layout Class
 * @author coco
 * @date   2020-03-18
 *************************************************/

#ifndef STATE_INSTRUMENTS_ER1_LAYOUT_H
#define STATE_INSTRUMENTS_ER1_LAYOUT_H

#include <memory>

#include "anemone/config.hpp"
#include "anemone/types.hpp"


// TODO make a base class for InstrumentLayout?
class ER1Layout : public Layout {
public:
  std::shared_ptr<GridSection> OSC1;
  std::shared_ptr<GridSection> OSC2;
  std::shared_ptr<GridSection> OSC3;
  std::shared_ptr<GridSection> OSC4;
  
  ER1Layout(std::shared_ptr<Config> config) : Layout(LayoutName::ER1) {

    auto layout = config->at("instruments")["er1"]["layout"];
      
    OSC1 = std::make_shared<GridSection>(GridSectionName::OSC1,
                                         layout.parse_grid_region("osc1"));
    OSC2 = std::make_shared<GridSection>(GridSectionName::OSC2,
                                         layout.parse_grid_region("osc2"));
    OSC3 = std::make_shared<GridSection>(GridSectionName::OSC3,
                                         layout.parse_grid_region("osc3"));
    OSC4 = std::make_shared<GridSection>(GridSectionName::OSC4,
                                         layout.parse_grid_region("osc4"));

    register_section(OSC1);
    register_section(OSC2);
    register_section(OSC3);
    register_section(OSC4);
  };
};


#endif
