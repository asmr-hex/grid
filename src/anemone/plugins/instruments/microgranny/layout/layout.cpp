#include "anemone/plugins/instruments/microgranny/layout/layout.hpp"


MicroGranny::MicroGrannyLayout::MicroGrannyLayout(std::shared_ptr<Config> config)
  : Layout(LayoutName::MicroGranny)
{
  auto layout = config->at("instruments")["microgranny"]["layout"];
      
  samples = std::make_shared<GridSection>(GridSectionName::MicroGrannySamples,
                                       layout.parse_grid_region("samples"));
  presets = std::make_shared<GridSection>(GridSectionName::MicroGrannyPresets,
                                       layout.parse_grid_region("presets"));
  banks   = std::make_shared<GridSection>(GridSectionName::MicroGrannyBanks,
                                       layout.parse_grid_region("banks"));
}

void MicroGranny::MicroGrannyLayout::register_sections() {
  register_section(samples);
  register_section(presets);
  register_section(banks);
}
