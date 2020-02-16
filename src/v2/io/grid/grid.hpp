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

#include "anemone/config/config.hpp"
#include "anemone/io/observer.hpp"
#include "anemone/io/grid/device/grid.hpp"
#include "anemone/io/grid/layout/context.hpp"


/// @brief High-Level class for interacting with a connected grid device.
///
/// @details
/// This class is a wrapper around a `GridDevice` providing more high level interactions
/// with the underlying device object. In particular, it stores all possible `GridDevice`
/// layouts and translates `grid_addr_t` messages from the application to appropriate
/// low-level `grid_coordinate_t` message using the currently active layout as a translator.
/// Additionally, this class derives from an `Observer<grid_event_t>` in order to receive
/// inbound `grid_event_t` messages coming from the `GridDevice` object. A simple API is
/// exposed for setting the brightness of specific leds on the device grid. Importantly,
/// the `Grid` object isn't itself an io `Observable<Event>`, but rather the layouts which
/// it contains are observable. Thus any user code which wishes to receive inbound messages
/// from the grid device must subscribe to the layout section of interest.
///
/// @todo include an animator which will control animations for grid device leds.
///
class Grid : public Observer<grid_device_event_t> {
public:
  /// @brief Provides access to the current layout and all possible layouts
  LayoutContext layout;

  /// @brief Constructs a high-level `Grid` object.
  ///
  /// @param config        pointer to a configuration object
  /// @param grid_device   pointer to a grid device object
  /// @param layouts       initializer list of all grid layouts
  ///
  Grid(std::shared_ptr<Config> config,
       std::shared_ptr<GridDevice> grid_device,
       layout_initializer_list layouts);

  /// @brief Connects and subscribes this object to the wrapped `GridDevice`.
  ///
  /// @details
  /// A `connect` method was introduced to decouple object construction from subscribing
  /// and connecting to the underlying `GridDevice`. While this provides more control for
  /// end-users for deciding when a connection should be established, it is also necessary
  /// given that the `subscribe` method of `Observer<Event>` requires that the Observer is
  /// a `shared_ptr`. Since the smart ptr to this object is not fully constructed while
  /// executing its constructor we are unable to use `this` reliably within the body of the
  /// constructor.
  ///
  void connect();
  
  
  /// @brief Turns off the led on the grid device at the specific grid address.
  ///
  /// @param address   the grid address of the led
  ///
  /// @details
  /// Before sending this address to the grid device, it is translated by the current layout
  /// to the appropriate grid device address.
  void turn_off(const grid_addr_t& address);

  /// @brief Turns on the led on the grid device at the specific grid address.
  ///
  /// @param address   the grid address of the led
  ///
  /// @details
  /// Before sending this address to the grid device, it is translated by the current layout
  /// to the appropriate grid device address.
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

  // on calls which use animator (like add animation, the addresses are translated and THEN stored in the animator) (animator has a shared_ptr of the GridDevice)
  // TODO implement animator...

protected:
  /// @brief handles inbound `grid_device_event_t`s from the grid device.
  ///
  /// @details
  /// This implements the `Observer<Event>`'s `handle` method. The received events are
  /// passed to the layout for further processing and broadcasting.
  ///
  virtual void handle(const grid_device_event_t&) override;

private:
  /// @brief the port address of the grid device
  std::string device_addr;

  /// @brief the `GridDevice` object pointer
  std::shared_ptr<GridDevice> device;
};

#endif
