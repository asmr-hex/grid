#include "anemone/anemone.hpp"

#include <spdlog/spdlog.h>


Anemone::Anemone(std::string config_path,
                 std::shared_ptr<GridDevice> grid_device,
                 std::shared_ptr<MidiDeviceFactory> midi_device_factory)
{
  spdlog::set_level(spdlog::level::info);
  spdlog::set_pattern("[%H:%M:%S:%e] [thread %t] %^[%l]%$ %v");

  spdlog::info("============= initialization  ============");

  // initialize configuration
  spdlog::info("  initializing \tconfiguration");
  config = std::make_shared<Config>(config_path);

  // initialize plugin-manager
  spdlog::info("  initializing \tplugin manager");
  plugin_manager = std::make_shared<PluginManager>(config);

  // initialize state
  spdlog::info("  initializing \tstate");
  state = std::make_shared<State>(State(config, plugin_manager));

  // initialize io
  spdlog::info("  initializing \tio");
  io = std::make_shared<IO>(IO(config, grid_device, midi_device_factory, state));

  // initialize controllers
  spdlog::info("  initializing \tcontrollers");
  controllers = std::make_shared<Controllers>(Controllers(io, state, plugin_manager));

  // initialize ui
  spdlog::info("  initializing \tui");
  ui = std::make_shared<UI>(UI(config, io, state, plugin_manager));
}

void Anemone::run() {
  spdlog::info("============= connecting ================");

  state->connect();

  io->connect();

  controllers->connect();

  ui->connect();

  // TODO deal with threads....
  while (true) {
    std::this_thread::sleep_for(std::chrono::minutes(5));
  }
}
