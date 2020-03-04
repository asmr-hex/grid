/**
 * @file   types/io/grid/animation.hpp
 * @brief  IO Grid Animation Types
 * @author coco
 * @date   2020-03-04
 *************************************************/


#ifndef TYPES_IO_GRID_ANIMATION_H
#define TYPES_IO_GRID_ANIMATION_H


#include <chrono>


class Animation {
public:
  virtual ~Animation() = default;

  /// @brief computes the pixel intensity (0-15) at the given time step.
  ///
  /// @param time_step    the time step.
  ///
  /// @return intensity   an unsigned int in [0, 15] representing the pixel value.
  ///
  virtual unsigned int compute(std::chrono::milliseconds time_step) = 0;
};

#endif
