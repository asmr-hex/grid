#ifndef IO_GRID_DEVICE_H
#define IO_GRID_DEVICE_H

#include <string>

#include "anemone/io/observable.hpp"
#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/device/coordinates.hpp"


class GridDevice : public Observable<grid_device_event_t> {
public:
  virtual ~GridDevice() = default;

  virtual void connect(std::string addr) = 0;
  virtual void listen() = 0;

  virtual void turn_off(grid_coordinates_t c) = 0;
  virtual void turn_on(grid_coordinates_t c) = 0;
  virtual void set(grid_coordinates_t c, unsigned int intensity) = 0;
};

#endif
