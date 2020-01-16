/**
 * @file   io/io.hpp
 * @brief  I/O Context Class
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_H
#define IO_H

#include <string>
#include <memory>

#include "anemone/io/grid/grid.hpp"
#include "anemone/io/midi/midi.hpp"
#include "anemone/io/grid/device/grid.hpp"
#include "anemone/io/midi/device/midi.hpp"
#include "anemone/io/grid/layout/context.hpp"


/// @brief Class for consolidating all I/O devices.
class IO {
public:
  /// @brief Constructs & configures an IO context object.
  ///
  /// @pre pointers to configuration and devices must already exist.
  ///
  /// @param config                    pointer to a configuration object
  /// @param grid_device               pointer to a grid device object
  /// @param midi_device               pointer to a midi device object
  /// @param layout_initializer_list   initializer list of all grid layouts
  ///
  /// @details
  /// Constructs Grid and Midi objects (which wrap their respective device objects
  /// providing a higher level of abstraction) which are publicly exposed IO members.
  ///
  IO(std::shared_ptr<Config> config,
     std::shared_ptr<GridDevice> grid_device,
     std::shared_ptr<MidiDevice> midi_device,
     layout_initializer_list layouts);

  /// @brief Connects Grid & Midi objects to their ports.
  void connect();
  
  std::shared_ptr<Grid> grid;
  std::shared_ptr<Midi> midi;
};

#endif
