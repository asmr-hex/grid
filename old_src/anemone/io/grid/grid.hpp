/**
 * @file   io/grid/grid.hpp
 * @brief  IO Grid API Class
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_H
#define IO_GRID_H

#include <string>
#include <memory>

#include "anemone/rx.hpp"
#include "anemone/config.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/io/grid/device/grid.hpp"
#include "anemone/io/grid/animation/animator.hpp"


/// @brief High-Level class for interacting with a connected grid device.
///
/// @details
/// This class is a wrapper around a `GridDevice` providing more high level interactions
/// with the underlying device object. In particular, it subscribes to the current `GridDevice`
/// layout and translates `grid_addr_t` messages from the application to appropriate
/// low-level `grid_coordinate_t` message using the currently active layout as a translator.
/// Additionally, the class subscribes to the stream of incoming `grid_device_event_t`s which
/// originate from the grid device, translates each event into a grid_event_t` and exposes the
/// resulting observable stream.
///
/// @todo include an animator which will control animations for grid device leds.
///
class Grid {
public:
  /// @brief api for applying animations to the grid.
  std::shared_ptr<Animator> animation;
  
  /// @brief Constructs a high-level `Grid` object.
  ///
  /// @param config        pointer to a configuration object
  /// @param grid_device   pointer to a grid device object
  /// @param layout        pointer to a layout context
  ///
  Grid(std::shared_ptr<Config> config,
       std::shared_ptr<GridDevice> grid_device,
       std::shared_ptr<GridLayouts> layouts);

  /// @brief Connects and subscribes this object to the wrapped `GridDevice`.
  ///
  /// @details
  /// A `connect` method was introduced to decouple object construction from subscribing
  /// and connecting to the underlying `GridDevice`. This provides more control for
  /// end-users for deciding when a connection should be established.
  ///
  /// return observable   a stream of `grid_event_t` observables.
  ///
  rx::observable<grid_event_t> connect();
  
  /// @brief Turns off the led on the grid device at the specific grid address.
  ///
  /// @param address   the grid address of the led
  ///
  /// @details
  /// Before sending this address to the grid device, it is translated by the current layout
  /// to the appropriate grid device address.
  ///
  void turn_off(const grid_addr_t& address);

  /// @brief Turns on the led on the grid device at the specific grid address.
  ///
  /// @param address   the grid address of the led
  ///
  /// @details
  /// Before sending this address to the grid device, it is translated by the current layout
  /// to the appropriate grid device address.
  ///
  void turn_on(const grid_addr_t& address);

  /// @brief Sets the brightness of the led on the grid device at the specific grid address.
  ///
  /// @param address     the grid address of the led
  /// @param intensity   the brightness intensity to set the led to
  ///
  /// @details
  /// Before sending this address to the grid device, it is translated by the current layout
  /// to the appropriate grid device address.
  void set(const grid_addr_t& address, unsigned int intensity);

  // TODO implement turn_off, turn_on, and set for `grid_addr_region_t`!!!!
  
  // on calls which use animator (like add animation, the addresses are translated and THEN stored in the animator) (animator has a shared_ptr of the GridDevice)
  // TODO implement animator...

private:
  /// @brief the port address of the grid device
  std::string device_addr;

  /// @brief the `GridDevice` object pointer
  std::shared_ptr<GridDevice> device;

  /// @brief pointer to the current layout
  std::shared_ptr<Layout> current_layout;

};

#endif
