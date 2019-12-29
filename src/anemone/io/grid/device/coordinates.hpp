#ifndef IO_GRID_DEVICE_COORDINATES_H
#define IO_GRID_DEVICE_COORDINATES_H

#include <functional>


struct grid_coordinates_t {
public:
  unsigned int x;
  unsigned int y;

  // override logical equality operator so coordinates can be used as a map key
  bool operator==(const grid_coordinates_t &m) const {
    return x == m.x && y == m.y;
  };

  // override logical less than  operator so coordinates can be used as a map key
  bool operator<(const grid_coordinates_t &m) const {
    if (x < m.x && y <= m.y) return true;
    if (x <= m.x && y < m.y) return true;

    return false;
  };
};

// the grid_coordinates_hasher is used as the hash function when grid_coordinates_t
// is used as the key to an unordered_map.
struct grid_coordinates_hasher {
  size_t operator()(const grid_coordinates_t &m) const
  {
    size_t h1 = std::hash<double>()(m.x);
    size_t h2 = std::hash<double>()(m.y);
    return (h1 ^ (h2 << 1));
  };
};


struct grid_region_t {
  grid_coordinates_t min;
  grid_coordinates_t max;

  bool operator==(const grid_region_t &rhs) const {
    return
      min == rhs.min &&
      max == rhs.max;
  };
};


#endif
