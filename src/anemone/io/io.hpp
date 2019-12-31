#ifndef IO_H
#define IO_H

#include <string>
#include <memory>

#include "anemone/io/grid/grid.hpp"
#include "anemone/io/midi/midi.hpp"
#include "anemone/io/grid/device/grid.hpp"
#include "anemone/io/midi/device/midi.hpp"
#include "anemone/io/grid/layout/context.hpp"


class IO {
public:
  IO(std::shared_ptr<Config>,
     std::shared_ptr<GridDevice>,
     std::shared_ptr<MidiDevice>,
     layout_initializer_list);
  
  void connect();
  
  std::shared_ptr<Grid> grid;
  std::shared_ptr<Midi> midi;
};

#endif
