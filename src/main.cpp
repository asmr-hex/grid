#include <iostream>

#include "sequencer.hpp"


int main(int argc, char *argv[]) {
  if ( argc != 2 ) {
    std::cout << "Error: you must provide the path to the configuration file!\n";
    return -1;
  }

  Sequencer *sequencer = new Sequencer(argv[1]);
  sequencer->start();
  
  return 0;
}
