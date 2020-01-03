#include <memory>
#include <iostream>

#include "anemone/anemone.hpp"
#include "anemone/io/grid/device/monome.hpp"
#include "anemone/io/midi/device/rtmidi.hpp"


int main(int argc, char *argv[]) {
  if ( argc != 2 ) {
    std::cout << "Error: you must provide the path to the configuration file!\n";
    return -1;
  }

  Anemone anemone(argv[1],
                  std::make_shared<Monome>(),
                  std::make_shared<RTMidi>());

  anemone.run();
  
  return 0;
}
