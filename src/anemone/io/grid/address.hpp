#ifndef IO_GRID_LAYOUT_ADDRESS_H
#define IO_GRID_LAYOUT_ADDRESS_H

#include "anemone/io/grid/layout/names.hpp"


typedef unsigned int grid_section_index;

struct grid_addr_t {
  LayoutName layout;
  GridSectionName section;
  grid_section_index index;
};

#endif
