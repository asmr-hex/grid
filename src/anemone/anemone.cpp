#include "anemone/anemone.hpp"


Anemone::Anemone(std::string config_path, std::shared_ptr<GridDevice> grid_device, std::shared_ptr<MidiDevice> midi_device)
  : config(config_path) {

  // initialize layouts
  layouts.sequencer = std::make_shared<GridLayout::Sequencer>(config);
  
  // setup io
  io = std::make_shared<IO>(config, grid_device, midi_device, {layouts.sequencer});
  
  // setup state and dispatcher (pre-req is the concurrent queue)
  // init_state();

  // setup clock

  // setup controllers
  
  // setup ui
}

void Anemone::run() {
  // start listening to io

  // start clock

  // 
}

void Anemone::init_state() {
  // auto queue = std::make_shared< Queue<action_t> >();

  // dispatcher = std::make_shared<Dispatcher>(queue);
  // state = std::make_shared<State::Tree::Root>(queue);
}
