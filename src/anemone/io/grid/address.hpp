/**
 * @file   io/grid/address.hpp
 * @brief  IO Grid Address
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_LAYOUT_ADDRESS_H
#define IO_GRID_LAYOUT_ADDRESS_H

#include "anemone/io/grid/layout/names.hpp"


/// @brief type alias for grid sectionn index.
typedef unsigned int grid_section_index;

/// @breif grid address.
struct grid_addr_t {
  LayoutName layout;
  GridSectionName section;
  grid_section_index index;
};

#endif
