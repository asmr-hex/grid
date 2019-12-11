#ifndef IO_GRID_H
#define IO_GRID_H

#include <string>

#include "../../config/mappings/types.hpp"

#include "../observable.hpp"
#include "types.hpp"


class Grid : public Observable<grid_event_t> {
public:
  virtual ~Grid() = default;

  virtual void connect(std::string addr) = 0;
  virtual void listen() = 0;

  virtual void turn_off(grid_addr_t) = 0;
  virtual void turn_on(grid_addr_t c) = 0;
  virtual void set(grid_addr_t c, unsigned int intensity) = 0;
};

#endif
