#ifndef ANEMONE_H
#define ANEMONE_H

#include <string>
#include <memory>


#include "anemone/io.hpp"
#include "anemone/ui.hpp"
#include "anemone/types.hpp"
#include "anemone/config.hpp"
#include "anemone/state.hpp"
#include "anemone/controllers.hpp"


class Anemone {
public:
  Anemone(std::string config_path,
          std::shared_ptr<GridDevice>,
          std::shared_ptr<MidiDeviceFactory>);
  
  void run();

  std::shared_ptr<Config> config;
  std::shared_ptr<IO> io;
  std::shared_ptr<UI> ui;
  std::shared_ptr<State> state;
  std::shared_ptr<Controllers> controllers;
};

#endif
