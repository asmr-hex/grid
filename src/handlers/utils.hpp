#ifndef HANDLER_UTILS_H
#define HANDLER_UTILS_H

#include <monome.h>

#include "../io/io.hpp"
#include "../config/mappings/types.hpp"


void set_led_region_intensity(IO *io, mapping_range_2d_t *range, unsigned int intensity) {
  for (unsigned int x=range->x.min; x<=range->x.max; x++) {
    for (unsigned int y=range->y.min; y<=range->y.max; y++) {
      monome_led_level_set(io->output.monome, x, y, intensity);
    }
  }
};

#endif
