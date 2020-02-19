/**
 * @file   types/io/grid/address.hpp
 * @brief  IO Grid Address Types
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef TYPES_IO_GRID_LAYOUT_ADDRESS_H
#define TYPES_IO_GRID_LAYOUT_ADDRESS_H

#include "anemone/types/layout/names.hpp"


/// @brief type alias for grid sectionn index.
typedef unsigned int grid_section_index;

/// @breif grid address.
struct grid_addr_t {
  LayoutName layout;
  GridSectionName section;
  grid_section_index index;
};

#endif
