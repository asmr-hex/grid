#ifndef MAPPING_TYPES_H
#define MAPPING_TYPES_H

struct mapping_range_1d_t {
  unsigned int min;
  unsigned int max;  
};

struct mapping_coordinates_t {
  unsigned int x;
  unsigned int y;
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
  
  unsigned int get_width() {
    if (width == 0) width = x.max - x.min;

    return width;
  };

  unsigned int get_height() {
    if (height == 0) height = y.max - y.min;

    return height;    
  };
  
private:
  unsigned int width;
  unsigned int height;
};

#endif
