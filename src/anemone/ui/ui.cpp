#include <spdlog/spdlog.h>

#include "anemone/ui/ui.hpp"


UI::UI(std::shared_ptr<Config> config,
       std::shared_ptr<IO> io,
       std::shared_ptr<State> state,
       std::shared_ptr<PluginManager> plugin_manager)
  : config(config),
    io(io),
    state(state),
    plugin_manager(plugin_manager)
{}


void UI::connect() {
  shift             = std::make_unique<ShiftUI>("SequencerAndInstrument", "Shift", io, state);
  step_sequence     = std::make_unique<StepSequenceUI>("SequencerAndInstrument", "Steps", io, state);
  pages             = std::make_unique<PageUI>("SequencerAndInstrument", "Pages", io, state);
  parts             = std::make_unique<PartsUI>("SequencerAndInstrument", "Parts", io, state);
  banks             = std::make_unique<BanksUI>("SequencerAndInstrument", "Banks", io, state);
  ppqn              = std::make_unique<PPQNUI>("SequencerAndInstrument", "PPQN", io, state);
  play_pause        = std::make_unique<PlayPauseUI>("SequencerAndInstrument", "PlayPause", io, state);
  stop              = std::make_unique<StopUI>("SequencerAndInstrument", "Stop", io, state);
  show_last_step    = std::make_unique<ShowLastStepUI>("SequencerAndInstrument", "LastStep", io, state);
  instrument_select = std::make_unique<InstrumentSelectUI>("SequencerAndInstrument", "InstrumentSelect", io, state);

  // create ui plugins
  for (auto plugin : plugin_manager->plugins) {
    ui_plugins.push_back(plugin->make_ui(io, state));
  }
  
  spdlog::info("  connected -> ui");
}
