#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/er1/er1.hpp"
#include "anemone/plugins/instruments/er1/ui/ui.hpp"


ER1::UI::UI(std::shared_ptr<Config> config,
            std::shared_ptr<IO> io,
            std::shared_ptr<State> state,
            std::shared_ptr<ER1> er1)
  : config(config),
    io(io),
    state(state)
{
  osc     = std::make_unique<OSC_UI>(LayoutName::ER1, GridSectionName::Oscillators, io, state, er1);
  cymbals = std::make_unique<CymbalsUI>(LayoutName::ER1, GridSectionName::Cymbals, io, state, er1);
}
