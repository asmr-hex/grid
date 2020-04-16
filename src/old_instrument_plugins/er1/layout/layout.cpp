#include "anemone/plugins/instruments/er1/layout/layout.hpp"


ER1::ER1Layout::ER1Layout(std::shared_ptr<Config> config)
  : Layout("ER1")
{
  auto layout = config->at("instruments")["er1"]["layout"];

  oscillators = std::make_shared<GridSection>("Oscillators",
                                              layout.parse_grid_region("oscillators"));
  cymbals     = std::make_shared<GridSection>("Cymbals",
                                              layout.parse_grid_region("cymbals"));
  audio_ins   = std::make_shared<GridSection>("AudioIns",
                                              layout.parse_grid_region("aux"));
  delay_depth = std::make_shared<GridSection>("DelayDepth",
                                              layout.parse_grid_region("delay_depth"));
  delay_time  = std::make_shared<GridSection>("DelayTime",
                                              layout.parse_grid_region("delay_time"));
  level       = std::make_shared<GridSection>("Level",
                                              layout.parse_grid_region("level"));
  pan         = std::make_shared<GridSection>("Pan",
                                              layout.parse_grid_region("pan"));
  low_boost   = std::make_shared<GridSection>("LowBoost",
                                              layout.parse_grid_region("low_boost"));
  pitch       = std::make_shared<GridSection>("Pitch",
                                              layout.parse_grid_region("pitch"));
  decay       = std::make_shared<GridSection>("Decay",
                                              layout.parse_grid_region("decay"));
  mod_type    = std::make_shared<GridSection>("ModType",
                                              layout.parse_grid_region("mod_type"));
  mod_depth   = std::make_shared<GridSection>("ModDepth",
                                              layout.parse_grid_region("mod_depth"));
  mod_speed   = std::make_shared<GridSection>("ModSpeed",
                                              layout.parse_grid_region("mod_speed"));
  wave_type   = std::make_shared<GridSection>("WaveType",
                                              layout.parse_grid_region("wave_type"));
}

void ER1::ER1Layout::register_sections() {
  register_section(oscillators);
  register_section(cymbals);
  register_section(audio_ins);
  register_section(delay_depth);
  register_section(delay_time);
  register_section(level);
  register_section(pan);
  register_section(low_boost);
  register_section(pitch);
  register_section(decay);
  register_section(mod_type);
  register_section(mod_depth);
  register_section(mod_speed);
  register_section(wave_type);
}
