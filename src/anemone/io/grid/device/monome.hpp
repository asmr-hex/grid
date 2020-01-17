/**
 * @file   io/grid/device/monnome.hpp
 * @brief  Monome Grid Implementation of Grid Device
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_MONOME_H
#define IO_GRID_MONOME_H

#include <string>

#include <monome.h>

#include "anemone/io/grid/device/grid.hpp"
#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/device/coordinates.hpp"


/// @brief Monnome grid device.
class Monome : public GridDevice {
public:
  virtual void connect(std::string addr) override;
  virtual void listen() override;

  virtual void turn_off(grid_coordinates_t c) override;
  virtual void turn_on(grid_coordinates_t c) override;
  virtual void set(grid_coordinates_t c, unsigned int intensity) override;

private:
  monome_t *monome;

  static void callback_wrapper(const monome_event_t *m_event, void *user_data);
  static grid_device_event_t transform(const monome_event_t *m_event);
};

#endif
