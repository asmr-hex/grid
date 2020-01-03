#include "anemone/anemone.hpp"

#include <spdlog/spdlog.h>


Anemone::Anemone(std::string config_path, std::shared_ptr<GridDevice> grid_device, std::shared_ptr<MidiDevice> midi_device) {
  spdlog::set_level(spdlog::level::debug);

  spdlog::info("============= initialization ============");
  
  // initialize configuration
  spdlog::info("  initializing \tconfiguration");
  config = std::make_shared<Config>(config_path);
  
  
  // initialize layouts (TODO move to GridLayouts constructor)
  spdlog::info("  initializing \tlayouts");
  layouts = std::make_shared<GridLayouts>(config);
  
  // initialize io
  spdlog::info("  initializing \tio");
  io = std::make_shared<IO>(IO(config, grid_device, midi_device, {layouts->sequencer}));

  // initialize state
  spdlog::info("  initializing \tstate");
  state = std::make_shared<State::Root>();

  // initialize dispatcher
  spdlog::info("  initializing \tdispatcher");
  dispatcher = std::make_shared<Dispatcher>(state);

  // setup clock
  spdlog::info("  initializing \tclock");
  clock = std::make_shared<Clock>(state);

  // setup controllers
  spdlog::info("  initializing \tcontrollers");
  controllers = std::make_shared<Controllers>
    (Controllers(io, clock, layouts, state, dispatcher));
  
  // setup ui

  spdlog::info("");
  spdlog::info("========== state initialization =========");
  dispatcher->connect();
  clock->connect_to_state();
  controllers->connect_to_state();
  dispatcher->hydrate();
}

void Anemone::run() {
  
  
  spdlog::info("============= connecting ================");
  io->connect();

  // clock is blocking (non-detatched thread)
  clock->start();
}
