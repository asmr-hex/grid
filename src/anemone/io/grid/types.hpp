#ifndef IO_GRID_TYPES_H
#define IO_GRID_TYPES_H

#include "../../config/mappings/types.hpp"


// grid event types correspond to a button being pressed down or lifted up.
enum grid_event_type_t { BUTTON_DOWN, BUTTON_UP };

// a grid event is a set of coordinates cooresponding to a button on the grid
// with an associated event type.
struct grid_event_t : public mapping_coordinates_t {
  grid_event_type_t type;
};

#endif
