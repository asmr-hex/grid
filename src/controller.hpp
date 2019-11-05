#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

#include "io/io.hpp"


class Controller {
public:
  Controller(std::string) {
    io = new IO();
  };
  void run();

private:
  IO *io;
  Config config;
  StateMachine state_machine;
  
  Sequencer sequencer;
  Instruments instruments;
};

#endif
