#include "anemone/state/layouts/sequencer.hpp"


GridLayout::Sequencer::Sequencer(std::shared_ptr<Config> config) :
      Layout(LayoutName::SequencerAndInstrument)
{
  auto layouts = config->at("layouts")["sequencer"]["layout"];
  
  instrument_select = std::make_shared<GridSection>(GridSectionName::InstrumentSelect,
                                                    layouts.parse_grid_region("instrument_select"));
  instrument_panel  = std::make_shared<GridSection>(GridSectionName::InstrumentPanel,
                                                    layouts.parse_grid_region("instrument_panel"));
  steps             = std::make_shared<GridSection>(GridSectionName::Steps,
                                                    layouts.parse_grid_region("steps"));
  pages             = std::make_shared<GridSection>(GridSectionName::Pages,
                                                    layouts.parse_grid_region("pages"));
  parts             = std::make_shared<GridSection>(GridSectionName::Parts,
                                                    layouts.parse_grid_region("parts"));
  banks             = std::make_shared<GridSection>(GridSectionName::Banks,
                                                    layouts.parse_grid_region("banks"));
  ppqn              = std::make_shared<GridSection>(GridSectionName::PPQN,
                                                    layouts.parse_grid_region("ppqn"));
  shift             = std::make_shared<GridSection>(GridSectionName::Shift,
                                                    layouts.parse_grid_region("shift"));
  play_pause        = std::make_shared<GridSection>(GridSectionName::PlayPause,
                                                    layouts.parse_grid_region("play_pause"));
  stop              = std::make_shared<GridSection>(GridSectionName::Stop,
                                                    layouts.parse_grid_region("stop"));
  record            = std::make_shared<GridSection>(GridSectionName::Record,
                                                    layouts.parse_grid_region("record"));
  last_step         = std::make_shared<GridSection>(GridSectionName::LastStep,
                                                    layouts.parse_grid_region("last_step"));
  metronome         = std::make_shared<GridSection>(GridSectionName::Metronome,
                                                    layouts.parse_grid_region("metronome"));
  
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

  // register_sub_layout();
}

