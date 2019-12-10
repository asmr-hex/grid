#ifndef IO_GRID_H
#define IO_GRID_H

#include <string>

#include "../../config/mappings/types.hpp"

#include "observer.hpp"


class Grid {
public:
  virtual void connect(std::string addr) = 0;
  virtual void register_observer(const std::shared_ptr<GridObserver>&) = 0;
  virtual void listen() = 0;

  virtual void turn_off(mapping_coordinates_t c) = 0;
  virtual void turn_on(mapping_coordinates_t c) = 0;
  virtual void set(mapping_coordinates_t c, unsigned int intensity) = 0;
};

#endif
