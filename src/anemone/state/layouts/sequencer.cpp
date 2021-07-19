#include <spdlog/spdlog.h>

#include "anemone/plugins.hpp"
#include "anemone/state/state.hpp"
#include "anemone/state/layouts/sequencer.hpp"


GridLayout::Sequencer::Sequencer(std::shared_ptr<Config> config, std::shared_ptr<PluginManager> plugin_manager)
  : Layout("SequencerAndInstrument"),
    plugin_manager(plugin_manager)
{
  auto layouts = config->at("layouts")["sequencer"]["layout"];

  control_select       = std::make_shared<GridSection>("ControlSelect",
                                                       layouts.parse_grid_region("control_select"));
  instrument_select    = std::make_shared<GridSection>("InstrumentSelect",
                                                       layouts.parse_grid_region("instrument_select"));
  instrument_panel     = std::make_shared<GridSection>("InstrumentPanel",
                                                    layouts.parse_grid_region("instrument_panel"));
  instrument_controls  = std::make_shared<GridSection>("InstrumentControls",
                                                    layouts.parse_grid_region("instrument_controls"));
  steps                = std::make_shared<GridSection>("Steps",
                                                       layouts.parse_grid_region("steps"));
  pages                = std::make_shared<GridSection>("Pages",
                                                       layouts.parse_grid_region("pages"));
  parts                = std::make_shared<GridSection>("Parts",
                                                       layouts.parse_grid_region("parts"));
  banks                = std::make_shared<GridSection>("Banks",
                                                       layouts.parse_grid_region("banks"));
  ppqn                 = std::make_shared<GridSection>("PPQN",
                                                       layouts.parse_grid_region("ppqn"));
  shift                = std::make_shared<GridSection>("Shift",
                                                       layouts.parse_grid_region("shift"));
  play_pause           = std::make_shared<GridSection>("PlayPause",
                                                       layouts.parse_grid_region("play_pause"));
  stop                 = std::make_shared<GridSection>("Stop",
                                                       layouts.parse_grid_region("stop"));
  record               = std::make_shared<GridSection>("Record",
                                                       layouts.parse_grid_region("record"));
  last_step            = std::make_shared<GridSection>("LastStep",
                                                       layouts.parse_grid_region("last_step"));
  metronome            = std::make_shared<GridSection>("Metronome",
                                                    layouts.parse_grid_region("metronome"));
}

void GridLayout::Sequencer::register_sections() {
  register_section(control_select);
  register_section(instrument_select);
  register_section(instrument_panel);
  register_section(instrument_controls);
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

  // register all instrument layouts as sublayouts
  for (auto instrument_plugin : plugin_manager->instrument_plugins->plugins) {
    register_sublayout("InstrumentPanel", instrument_plugin->get_layout());
  }
  
  // add handler for sublayout updates
  add_sublayout_update_handler
    ([this] (std::shared_ptr<State> state) {
       state->instruments->rendered.get_observable()
         .subscribe([this] (std::shared_ptr<Instrument> instrument) {
                      // update instrument_panel sublayout
                      update_sublayout_for("InstrumentPanel",
                                           instrument->layout);
                    });       
     });
}
