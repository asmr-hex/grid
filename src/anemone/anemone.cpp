#include "anemone/anemone.hpp"


Anemone::Anemone(std::string config_path, GridDevice grid_device, MidiDevice midi_device)
  : config(config_path), io(std::move(grid), std::move(midi)) {
  // read config

  // setup layouts
  
  // setup state and dispatcher (pre-req is the concurrent queue)
  init_state();

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
