/**
 * @file   io/grid/device/coordinates.hpp
 * @brief  IO Grid Device Coordinate Types
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_DEVICE_COORDINATES_H
#define IO_GRID_DEVICE_COORDINATES_H

#include <functional>


/// @brief Coordinates on a grid device.
struct grid_coordinates_t {
public:
  /// @brief x coordinate on the grid device
  unsigned int x;

  /// @brief y coordinate on the grid device
  unsigned int y;

  /// @brief grid coordinate equality operator.
  ///
  /// @remark
  /// Providing an equality (`==`) operator definition is necessary for using this
  /// struct as a key in a std lib map collection.
  ///
  bool operator==(const grid_coordinates_t &m) const {
    return x == m.x && y == m.y;
  };

  /// @brief grid coordinate logical less-than operator.
  ///
  /// @remark
  /// Providing a less-than (`<`) operator definition is necessary for using this
  /// struct as a key in a std lib map collection.
  ///
  bool operator<(const grid_coordinates_t &m) const {
    if (x <  m.x && y <= m.y) return true;
    if (x <= m.x && y <  m.y) return true;

    return false;
  };
};


/// @brief a grid coordinates hasher functor.
///
/// @remark
/// In order to use `grid_coordinates_t` as a std lib unnordered map key, it is necessary
/// to provide a hasher function to the map constructor.
///
struct grid_coordinates_hasher {
  /// @brief override `()` operator for hash function.
  size_t operator()(const grid_coordinates_t &m) const
  {
    size_t h1 = std::hash<double>()(m.x);
    size_t h2 = std::hash<double>()(m.y);
    return (h1 ^ (h2 << 1));
  };
};


/// @brief A rectangular region of grid device coordinates.
struct grid_region_t {
  /// @brief minimum x/y coordinates of the region
  grid_coordinates_t min;

  /// @brief maximum x/y coordinates of the region
  grid_coordinates_t max;

  /// @brief supports equality comparison of grid device regions.
  bool operator==(const grid_region_t &rhs) const {
    return
      min == rhs.min &&
      max == rhs.max;
  };
};


#endif
