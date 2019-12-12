#ifndef IO_GRID_SECTION_H
#define IO_GRID_SECTION_H

#include "anemone/io/grid/device/coordinates.hpp"


enum class
GridSectionName {
                 PlayPause,
                 Stop,
                 Record,
                 Metronome,
                 
                 Shift,
                 
                 InstrumentSelect,
                 InstrumentPanel,
                 
                 Steps,
                 Pages,
                 
                 Parts,
                 Banks,
                 
                 PPQN,
};


typedef unsigned int grid_section_index;


struct grid_addr_t {
  GridSectionName section;
  grid_section_index index;
};


class GridSection {
public:
  GridSectionName name;

  GridSection(GridSectionName name, const grid_region_t& region)
    : name(name), region_m(region) {};

  unsigned int width();
  unsigned int height();
  unsigned int size();
  grid_region_t region() { return region_m; };

  grid_coordinates_t coordinates_of(unsigned int index);
  std::vector<grid_coordinates_t> coordinates_in();
  std::vector<grid_coordinates_t> coordinates_in(unsigned int start, unsigned int end);

  unsigned int index_of(const grid_coordinates_t& coordinates);
  
private:
  grid_region_t region_m;
  
  unsigned int width_m  = 0;
  unsigned int height_m = 0;
  unsigned int size_m   = 0;
};
  
#endif
