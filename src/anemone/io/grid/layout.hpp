#ifndef IO_GRID_LAYOUT_H
#define IO_GRID_LAYOUT_H


typedef unsigned int grid_section_index;


struct grid_addr_t {
  GridSectionName section;
  grid_section_index index;
};


#endif
