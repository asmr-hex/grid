#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/er1/ui/ui.hpp"


ER1::UI::UI(std::shared_ptr<Config> config,
             std::shared_ptr<IO> io,
             std::shared_ptr<State> state)
  : config(config),
    io(io),
    state(state)
{
  osc1 = std::make_unique<OSC1_UI>(LayoutName::ER1, GridSectionName::OSC1, io, state);
}
