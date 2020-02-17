/**
 * @file   io/grid/device/grid.hpp
 * @brief  IO Grid Device Interface
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_DEVICE_H
#define IO_GRID_DEVICE_H

#include <string>

#include <rxcpp/rx.hpp>

#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/device/coordinates.hpp"


/// @brief Interface for grid devices.
///
/// @details
/// This class defines the minimal interface for grid devices. It defines
/// pure virtual methods for connecting to the underlying device, listening
/// for input from the underlying device, and interacting with the underlying
/// device in three ways: turning a grid led on, off, and setting its brightness.
/// This interface allows for dependency injection so that different grids (either
/// different models or mock grids for testing) can be swapped in.
/// Additionally, this interface is a `grid_device_event_t` observable, so observers
/// can subscribe to it to receive raw messages from the grid device.
///
// class GridDevice : public Observable<grid_device_event_t> {
class GridDevice : public rxcpp::subjects::subject<grid_device_event_t> {
public:
  virtual ~GridDevice() = default;

  /// @brief Establishes a connection with the underlying device.
  ///
  /// @param addr   the port address of the underlying device
  ///
  virtual void connect(std::string addr) = 0;

  /// @brief listens for inbound messages from the underlying device
  ///
  /// @details
  /// This is where any classes implementing this interface would broadcast new
  /// messages coming from the grid device.
  ///
  virtual rxcpp::observable<grid_device_event_t> listen() = 0;

  /// @brief turns off the led at the provided device coordinate.
  ///
  /// @param coordinates   the location of the led on the actual device
  ///
  virtual void turn_off(grid_coordinates_t coordinates) = 0;

  /// @brief turns on the led at the provided device coordinate.
  ///
  /// @param coordinates   the location of the led on the actual device
  ///
  virtual void turn_on(grid_coordinates_t coordinates) = 0;

  /// @brief sets the brightness of the led at the provided device coordinate.
  ///
  /// @param coordinates   the location of the led on the actual device
  /// @param intensity      an int representing the brightness intennsity of the led
  ///
  virtual void set(grid_coordinates_t coordinates, unsigned int intensity) = 0;
};

#endif
