#ifndef STATE_GRID_LAYOUT_SECTION_H
#define STATE_GRID_LAYOUT_SECTION_H

#include "anemone/types.hpp"


class GridSection {
public:
  GridSectionName name;

  GridSection() = default;
  GridSection(GridSectionName name, const grid_region_t& region)
    : name(name), region_m(region) {
    // calculate width, height, size
    width_m = region_m.max.x - region_m.min.x + 1;
    height_m = region_m.max.y - region_m.min.y + 1;
    size_m = width() * height();
  };

  unsigned int width() const;
  
  unsigned int height() const;
  unsigned int size() const;
  grid_region_t region() const { return region_m; };

  // should the below be protected? i.e. will anyone have to use them except for derived classes?
  grid_coordinates_t coordinates_of(unsigned int index) const;
  std::vector<grid_coordinates_t> coordinates_in() const;
  std::vector<grid_coordinates_t> coordinates_in(unsigned int start, unsigned int end) const;

  bool contains(const grid_coordinates_t& c) const;
  
  unsigned int index_of(const grid_coordinates_t& coordinates) const;

private:
  grid_region_t region_m;
  
  unsigned int width_m  = 0;
  unsigned int height_m = 0;
  unsigned int size_m   = 0;
};
  
#endif
