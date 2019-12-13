#ifndef IO_GRID_EVENT_H
#define IO_GRID_EVENT_H

#include "anemone/io/grid/address.hpp"


enum class GridEvent { Pressed, Unpressed };

struct grid_event_t : public grid_addr_t {
  GridEvent type;
};

#endif
