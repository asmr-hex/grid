#ifndef CONTROLLER_MAPPING
#define CONTROLLER_MAPPING

struct mapping_range_1d_t {
  unsigned int min;
  unsigned int max;  
};

struct mapping_range_2d_t {
  mapping_range_1d_t x;
  mapping_range_1d_t y;
};

struct mapping_coordinates_t {
  unsigned int x;
  unsigned int y;
};

struct ControllerMapping {
  mapping_range_2d_t steps;
  
};

#endif
