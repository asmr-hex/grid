#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/microgranny/microgranny.hpp"
#include "anemone/plugins/instruments/microgranny/ui/ui.hpp"


MicroGranny::UI::UI(std::shared_ptr<Config> config,
            std::shared_ptr<IO> io,
            std::shared_ptr<State> state,
            std::shared_ptr<MicroGranny> microgranny_state)
  : config(config),
    io(io),
    state(state)
{
  samples = std::make_unique<SamplesUI>(LayoutName::MicroGranny, GridSectionName::MicroGrannySamples, io, state, microgranny_state);
}
