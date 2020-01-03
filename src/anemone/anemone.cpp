#include "anemone/anemone.hpp"


Anemone::Anemone(std::string config_path, std::shared_ptr<GridDevice> grid_device, std::shared_ptr<MidiDevice> midi_device) {

  // initialize configuration
  config = std::make_shared<Config>(config_path);
  
  // initialize layouts (TODO move to GridLayouts constructor)
  layouts->sequencer = std::make_shared<GridLayout::Sequencer>(config);
  
  // initialize io
  io = std::make_shared<IO>(IO(config, grid_device, midi_device, {layouts->sequencer}));

  // initialize state
  state = std::make_shared<State::Root>();

  // initialize dispatcher
  dispatcher = std::make_shared<Dispatcher>(state);

  // setup clock
  clock = std::make_shared<Clock>(state);

  // setup controllers
  controllers = std::make_shared<Controllers>
    (Controllers(io, clock, layouts, state, dispatcher));
  
  // setup ui
}

void Anemone::run() {
  io->connect();

  clock->start();
}
