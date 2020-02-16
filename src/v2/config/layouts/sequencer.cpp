#include "anemone/config/layouts/sequencer.hpp"
#include "anemone/io/grid/device/coordinates.hpp"


Conf::Layout::Sequencer::Sequencer(Conf::Base base) : Conf::Base(base) {
  instrument_select = GridSection(GridSectionName::InstrumentSelect,
                                  parse_grid_region("instrument_select"));
  instrument_panel  = GridSection(GridSectionName::InstrumentPanel,
                                  parse_grid_region("instrument_panel"));
  steps             = GridSection(GridSectionName::Steps,
                                  parse_grid_region("steps"));
  pages             = GridSection(GridSectionName::Pages,
                                  parse_grid_region("pages"));
  parts             = GridSection(GridSectionName::Parts,
                                  parse_grid_region("parts"));
  banks             = GridSection(GridSectionName::Banks,
                                  parse_grid_region("banks"));
  ppqn              = GridSection(GridSectionName::PPQN,
                                  parse_grid_region("ppqn"));
  shift             = GridSection(GridSectionName::Shift,
                                  parse_grid_region("shift"));
  play_pause        = GridSection(GridSectionName::PlayPause,
                                  parse_grid_region("play_pause"));
  stop              = GridSection(GridSectionName::Stop,
                                  parse_grid_region("stop"));
  record            = GridSection(GridSectionName::Record,
                                  parse_grid_region("record"));
  last_step         = GridSection(GridSectionName::LastStep,
                                  parse_grid_region("last_step"));
  metronome         = GridSection(GridSectionName::Metronome,
                                  parse_grid_region("metronome"));
}


grid_region_t Conf::Layout::Sequencer::parse_grid_region(std::string name) {
  auto layout = (*this)["layout"];
  
  if ( layout[name].yml.IsNull() ) throw("grid region '" + name + "' undefined!");
  
  YAML::Node x = layout[name]["x"].yml;
  YAML::Node y = layout[name]["y"].yml;

  unsigned int x_min, x_max, y_min, y_max;

  if ( x.IsScalar() ) {
    x_min = x.as<unsigned int>();
    x_max = x.as<unsigned int>();
  } else {
    x_min = x["min"].as<unsigned int>();
    x_max = x["max"].as<unsigned int>();    
  }

  if ( y.IsScalar() ) {
    y_min = y.as<unsigned int>();
    y_max = y.as<unsigned int>();
  } else {
    y_min = y["min"].as<unsigned int>();
    y_max = y["max"].as<unsigned int>();    
  }

  return {
          .min = { .x = x_min, .y = y_min},
          .max = { .x = x_max, .y = y_max},
  };
}
