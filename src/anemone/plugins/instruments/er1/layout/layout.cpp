#include "anemone/plugins/instruments/er1/layout/layout.hpp"


ER1::ER1Layout::ER1Layout(std::shared_ptr<Config> config)
  : Layout(LayoutName::ER1)
{
  auto layout = config->at("instruments")["er1"]["layout"];
      
  oscillators = std::make_shared<GridSection>(GridSectionName::Oscillators,
                                       layout.parse_grid_region("oscillators"));
}

void ER1::ER1Layout::register_sections() {
  register_section(oscillators);
}
