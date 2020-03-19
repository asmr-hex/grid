#include "anemone/config/layouts/sequencer.hpp"
#include "anemone/types/io/grid/device/coordinates.hpp"


Conf::Layout::Sequencer::Sequencer(Conf::Base base) : Conf::LayoutBase(base) {
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
