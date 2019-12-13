#include <utility>
#include <iostream>

#include "anemone/anemone.hpp"
#include "anemone/io/grid/device/monome.hpp"
#include "anemone/io/midi/rtmidi.hpp"


int main(int argc, char *argv[]) {
  if ( argc != 2 ) {
    std::cout << "Error: you must provide the path to the configuration file!\n";
    return -1;
  }

  // Anemone *anemone = new Anemone(argv[1],
  //                                new Monome(),
  //                                new RTMidi());
  // anemone->run();

  Anemone anemone(argv[1],
                  Monome(),
                  RTMidi());

  anemone.run();
  
  return 0;
}
