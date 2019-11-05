#include <iostream>

#include "controller.hpp"


int main(int argc, char *argv[]) {
  if ( argc != 2 ) {
    std::cout << "Error: you must provide the path to the configuration file!\n";
    return -1;
  }

  Controller *controller = new Controller(argv[1]);
  controller->run();
  
  return 0;
}
