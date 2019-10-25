#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "grid.hh"
#include "keyboard.hh"
#include "knobs.hh"

class Controller {
public:
  Controller();
  void start();
  
private:
  Grid *grid;
  Keyboard *keyboard;
  Knobs *knobs;
};

#endif
