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

/// @brief a grid address hasher functor.
///
/// @remark
/// In order to use `grid_addr_t` as a std lib unnordered map key, it is necessary
/// to provide a hasher function to the map constructor.
///
struct grid_addr_hasher {
  /// @brief override `()` operator for hash function.
  size_t operator()(const grid_addr_t &m) const
  {
    size_t h1 = std::hash<LayoutName>()(m.layout);
    size_t h2 = std::hash<GridSectionName>()(m.section);
    size_t h3 = std::hash<grid_section_index>()(m.index);
    return h1 ^ (h2 ^ (h3 << 1));
  };
};


#endif
