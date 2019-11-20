#ifndef CONTROLLER_MAPPING_H
#define CONTROLLER_MAPPING_H

#include "types.hpp"


struct ControllerMapping {
  mapping_range_2d_t instrument_select;
  mapping_range_2d_t instrument_region;
  
  mapping_range_2d_t steps;
  mapping_range_2d_t pages;
  
  mapping_range_2d_t parts;
  mapping_range_2d_t banks;

  mapping_range_2d_t ppqn;

  mapping_coordinates_t shift;
  mapping_coordinates_t play_pause;
  mapping_coordinates_t stop;
  mapping_coordinates_t record;
  mapping_coordinates_t last_step;
  mapping_coordinates_t tempo;
};

#endif
