#ifndef IO_GRID_DEVICE_EVENTS_H
#define IO_GRID_DEVICE_EVENTS_H

#include "anemone/io/grid/device/coordinates.hpp"


enum class GridDeviceEvent { ButtonUp, ButtonDown };

struct grid_device_event_t : public grid_coordinates_t {
  GridDeviceEvent type;
};

#endif
