#include "anemone/anemone.hpp"


Anemone::Anemone(Grid&& grid, Midi&& midi) : io(std::move(grid), std::move(midi)) {
  // read config
  
  // setup state and dispatcher (pre-req is the concurrent queue)

  // setup clock

  // setup ui
}
