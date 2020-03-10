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

#include "anemone/rx.hpp"
#include "anemone/state.hpp"

#include "anemone/io/clock/clock.hpp"
#include "anemone/io/grid/grid.hpp"
#include "anemone/io/grid/device/grid.hpp"
#include "anemone/io/midi/midi.hpp"
#include "anemone/io/midi/device/factory.hpp"


/// @brief Class for consolidating all I/O devices.
class IO {
public:
  /// @brief Constructs & configures an IO context object.
  ///
  /// @pre pointers to configuration and devices must already exist.
  ///
  /// @param config        pointer to a configuration object
  /// @param grid_device   pointer to a grid device object
  /// @param midi_device   pointer to a midi device object
  /// @param state         pointer to the state
  ///
  /// @details
  /// Constructs Grid and Midi objects (which wrap their respective device objects
  /// providing a higher level of abstraction) which are publicly exposed IO members.
  ///
  IO(std::shared_ptr<Config>,
     std::shared_ptr<GridDevice>,
     std::shared_ptr<MidiDeviceFactory> midi_device_factory,
     std::shared_ptr<State>);

  /// @brief Connects Grid & Midi objects to their ports.
  void connect();

  std::shared_ptr<Clock> clock;
  std::shared_ptr<Grid> grid;
  std::shared_ptr<Midi> midi;

  /// @brief observable stream of grid events
  rx::observable<grid_event_t> grid_events;

  /// @brief observable stream of midi events
  rx::observable<midi_event_t> midi_events;
  
  /// @brief observable stream of clock events
  rx::observable<tick_t> clock_events;
};

#endif
