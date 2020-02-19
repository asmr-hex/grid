#include "anemone/anemone.hpp"

#include <spdlog/spdlog.h>


Anemone::Anemone(std::string config_path,
                 std::shared_ptr<GridDevice> grid_device)
{
  spdlog::set_level(spdlog::level::info);

  spdlog::info("============= initialization ============");
  
  // initialize configuration
  spdlog::info("  initializing \tconfiguration");
  config = std::make_shared<Config>(config_path);

  // initialize state
  spdlog::info("  initializing \tstate");
  state = std::make_shared<State>(State(config));
  
  // initialize io
  spdlog::info("  initializing \tio");
  io = std::make_shared<IO>(IO(config, grid_device, state->layout));
  // io = std::make_shared<IO>(IO(config, grid_device, midi_device_factory, {layouts->sequencer}));

  // initialize controllers
  spdlog::info("  initializing \tcontrollers");
  controllers = std::make_shared<Controllers>(Controllers(io, state));

  // initialize ui
  spdlog::info("  initializing \tui");
  ui = std::make_shared<UI>(UI(config, io, state));
}

void Anemone::run() {
  spdlog::info("============= connecting ================");

  io->connect();

  controllers->connect();

  ui->connect();

  // wait for 20 seconds
  rx::observable<>::timer(std::chrono::milliseconds(20000)).
    subscribe( [&] (long) {});
}
