#include <vector>

#include "section.hpp"
#include "coordinates.hpp"


unsigned int GridSection::width() {
  if (width_m == 0) width_m = region_m.max.x - region_m.min.x + 1;

  return width_m;
}

unsigned int GridSection::height() {
  if (height_m == 0) height_m = region_m.max.y - region_m.min.y + 1;

  return height_m;
}

unsigned int GridSection::size() {
  if (size_m == 0) size_m = width() * height();

  return size_m;
}

grid_coordinates_t GridSection::coordinates_of(unsigned int index) {
  return {
          (index % width()) + region_m.min.x,
          (index / width()) + region_m.min.y,
  };
}

std::vector<grid_coordinates_t> GridSection::coordinates_in() {
  return coordinates_in(0, size());
}

std::vector<grid_coordinates_t> GridSection::coordinates_in(unsigned int start, unsigned int end) {
  std::vector<grid_coordinates_t> coordinates;

  for (int i=start; i<=end; i++) {
    coordinates.push_back(coordinates_of(i));
  }

  return coordinates;
}

unsigned int GridSection::index_of(const grid_coordinates_t& coordinates) {
  // normalize
  unsigned int x = coordinates.x - region_m.min.x;
  unsigned int y = coordinates.y - region_m.min.y;
    
  return ( y * width() ) + x;
}
