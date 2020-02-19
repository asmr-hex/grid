/**
 * @file   io/grid/device/monnome.hpp
 * @brief  Monome Grid Implementation of Grid Device
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_MONOME_H
#define IO_GRID_MONOME_H

#include <thread>
#include <string>

#include <monome.h>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"

#include "anemone/io/grid/device/grid.hpp"


/// @brief Monome grid device.
class Monome : public GridDevice {
public:
  virtual void connect(std::string addr) override;
  virtual rx::observable<grid_device_event_t> listen() override;

  virtual void turn_off(grid_coordinates_t c) override;
  virtual void turn_on(grid_coordinates_t c) override;
  virtual void set(grid_coordinates_t c, unsigned int intensity) override;

private:
  monome_t *monome;
};

#endif
