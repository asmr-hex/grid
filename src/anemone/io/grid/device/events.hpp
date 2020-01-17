/**
 * @file   io/grid/device/events.hpp
 * @brief  IO Grid Device Event Types
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_DEVICE_EVENTS_H
#define IO_GRID_DEVICE_EVENTS_H

#include "anemone/io/grid/device/coordinates.hpp"


/// @brief Event types for grid devices.
enum class GridDeviceEvent {
                            /// a grid button was unpressed
                            ButtonUp,
                            /// a grid button was pressed
                            ButtonDown
};


/// @brief Grid device event.
///
/// @details
/// This struct describes the location associated with a grid device event in terms of
/// the device coordinates of the button involved in the event.
///
struct grid_device_event_t : public grid_coordinates_t {
  /// @brief type of the grid device event.
  GridDeviceEvent type;

  /// @brief Helper function for creating grid device event structs.
  ///
  /// @param xi      x coordinate of the device event
  /// @param yi      y coordinate of the device event
  /// @param event   type of the grid device event
  static grid_device_event_t make(unsigned int xi,
                                  unsigned int yi,
                                  GridDeviceEvent type)
  {
    return {{
             .x   = xi,
             .y   = yi
            },
            .type = type,
    };
  }
};

#endif
