#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sequencer.hpp"
#include "grid.hpp"
#include "keyboard.hpp"
#include "knobs.hpp"

class Controller {
public:
  // Controller();
  void start();
  
private:
  Sequencer* sequencer;
  Grid* grid;
  Keyboard* keyboard;
  Knobs* knobs;
};

#endif
