#include <vector>

#include "anemone/types/io/grid/layout/section.hpp"


unsigned int GridSection::width() const {
  return width_m;
}

unsigned int GridSection::height() const {
  return height_m;
}

unsigned int GridSection::size() const {
  return size_m;
}

grid_coordinates_t GridSection::coordinates_of(unsigned int index) const {
  return {
          (index % width()) + region_m.min.x,
          (index / width()) + region_m.min.y,
  };
}

std::vector<grid_coordinates_t> GridSection::coordinates_in() const {
  return coordinates_in(0, size() - 1);
}

std::vector<grid_coordinates_t> GridSection::coordinates_in(unsigned int start, unsigned int end) const {
  std::vector<grid_coordinates_t> coordinates;

  for (int i=start; i<=end; i++) {
    coordinates.push_back(coordinates_of(i));
  }

  return coordinates;
}

bool GridSection::contains(const grid_coordinates_t& c) const {
  return
    c.x >= region_m.min.x &&
    c.x <= region_m.max.x &&
    c.y >= region_m.min.y &&
    c.y <= region_m.max.y;
    
}

unsigned int GridSection::index_of(const grid_coordinates_t& coordinates) const {
  // normalize
  unsigned int x = coordinates.x - region_m.min.x;
  unsigned int y = coordinates.y - region_m.min.y;
    
  return ( y * width() ) + x;
}
