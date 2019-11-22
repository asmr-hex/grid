#ifndef MAPPING_TYPES_H
#define MAPPING_TYPES_H

#include <vector>


struct mapping_range_1d_t {
  unsigned int min;
  unsigned int max;  
};

struct mapping_coordinates_t {
  unsigned int x;
  unsigned int y;

  // overload logical equality operator so coordinates can be used as a map key
  bool operator==(const mapping_coordinates_t &m) const {
    return x == m.x && y == m.y;
  };

  // overload logical less than  operator so coordinates can be used as a map key
  bool operator<(const mapping_coordinates_t &m) const {
    return x < m.x && y < m.y;
  };
};

class mapping_range_2d_t {
public:
  mapping_range_1d_t x;
  mapping_range_1d_t y;
  
  unsigned int get_sequential_index_from_coordinates(unsigned int xi, unsigned int yi) {
    // normalize input
    xi -= x.min;
    yi -= y.min;
    
    return (yi * get_width()) + xi;
  }

  mapping_coordinates_t get_coordinates_from_sequential_index(unsigned int idx) {
    return {
            (idx % get_width()) + x.min,
            (idx / get_width()) + y.min,
    };
  };

  std::vector<mapping_coordinates_t> get_region_coordinates(unsigned int start, unsigned int end) {
    std::vector<mapping_coordinates_t> region;

    for (int i=start; i<=end; i++) {
      region.push_back(get_coordinates_from_sequential_index(i));
    }

    return region;
  };
  
  unsigned int get_width() {
    if (width == 0) width = x.max - x.min + 1;

    return width;
  };

  unsigned int get_height() {
    if (height == 0) height = y.max - y.min + 1;

    return height;
  };

  unsigned int get_area() {
    if (area == 0) area = get_width() * get_height();

    return area;
  }
  
private:
  unsigned int width;
  unsigned int height;
  unsigned int area;
};

#endif
