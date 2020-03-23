#include <spdlog/spdlog.h>

#include "anemone/plugins.hpp"
#include "anemone/state/state.hpp"


State::State(std::shared_ptr<Config> config, std::shared_ptr<PluginManager> plugin_manager)
  : layouts(std::make_shared<GridLayouts>(config, plugin_manager)),
    controls(std::make_shared<GlobalControls>(config)),
    instruments(std::make_shared<Instruments>(config, plugin_manager->instrument_plugins))
{}

void State::connect() {
  spdlog::warn("ABOUT TO CONNECT TO LAYOUTS");

  layouts->connect(shared_from_this());

  spdlog::info("  connected -> state");
}
