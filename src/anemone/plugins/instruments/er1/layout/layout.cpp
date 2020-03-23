#include "anemone/plugins/instruments/er1/layout/layout.hpp"


ER1::ER1Layout::ER1Layout(std::shared_ptr<Config> config)
  : Layout(LayoutName::ER1)
{
  auto layout = config->at("instruments")["er1"]["layout"];
      
  OSC1 = std::make_shared<GridSection>(GridSectionName::OSC1,
                                       layout.parse_grid_region("osc1"));
  OSC2 = std::make_shared<GridSection>(GridSectionName::OSC2,
                                       layout.parse_grid_region("osc2"));
  OSC3 = std::make_shared<GridSection>(GridSectionName::OSC3,
                                       layout.parse_grid_region("osc3"));
  OSC4 = std::make_shared<GridSection>(GridSectionName::OSC4,
                                       layout.parse_grid_region("osc4"));
}

void ER1::ER1Layout::register_sections() {
  register_section(OSC1);
  register_section(OSC2);
  register_section(OSC3);
  register_section(OSC4);
}
