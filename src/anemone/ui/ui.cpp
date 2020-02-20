#include <spdlog/spdlog.h>

#include "anemone/ui/ui.hpp"


UI::UI(std::shared_ptr<Config> config, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : config(config),
    io(io),
    state(state)
{}


void UI::connect() {
  shift = std::make_unique<ShiftUI>(LayoutName::SequencerAndInstrument, GridSectionName::Shift, io, state);

  spdlog::info("  connected -> ui");
}
