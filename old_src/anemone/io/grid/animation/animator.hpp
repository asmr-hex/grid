/**
 * @file   io/grid/animation/animator.hpp
 * @brief  IO Grid Animator API Class
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_ANIMATION_ANIMATOR_H
#define IO_GRID_ANIMATION_ANIMATOR_H

#include <mutex>
#include <memory>
#include <unordered_map>

#include "anemone/rx.hpp"
#include "anemone/config.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"
#include "anemone/io/grid/device/grid.hpp"


/// @brief The `Animator` class exposes an API for running animations on the grid.
///
/// @remark the animator uses a `GridDevice` to output commands to the grid for animation.
/// it also takes a pointer to the current layout to perform translations from `grid_addr_t`
/// to `grid_coordinates_t`. It would be easier, implemetation-wise, to just take a `Grid`
/// which performs those translations within the command methods, but that would mean that
/// the translations would be occuring everytime we send an animation command, which is very
/// frequent. so, a more efficient approach is to translate inbound `animation_msgs` one time
/// and store the `grid_coordinates_t` within the animation registry so that we don't have to
/// perform this animation on every frame update.
///
class Animator {
public:
  /// @brief constructs an animator.
  ///
  /// @param grid_device      a pointer to a grid device
  /// @param current_layout   a pointer to the current layout
  ///
  Animator(std::shared_ptr<GridDevice>, std::shared_ptr<Layout>);

  /// @brief begins tha animation loop.
  void run();

  /// @brief add an animation to a grid address.
  ///
  /// @param animation    a pointer to an animation.
  /// @param grid_addr    the grid address to add the animation to.
  ///
  void add(std::shared_ptr<Animation>, grid_addr_t);

  /// @brief add multiple animations to grid addresses in bulk.
  ///
  /// @param grid_add_to_animation   a map from grid address to animation.
  ///
  void add(std::unordered_map<grid_addr_t, std::shared_ptr<Animation>, grid_addr_hasher>);

  /// @brief remove an animation from a provided grid address.
  ///
  /// @param grid_addr   the grid address to remove the animation from.
  ///
  void remove(grid_addr_t);

  /// @brief remove an animation from a provided grid address and set a constant value.
  ///
  /// @param grid_addr   the grid address to remove the animation from.
  /// @param intensity   the constant intensity to set the grid address to.
  ///
  void remove(grid_addr_t, unsigned int);

  /// @brief bulk remove animations from the given addresses.
  ///
  /// @param grid_addr_list   a vector of grid addresses to remove animations from.
  ///
  void remove(std::vector<grid_addr_t>);

  /// @brief bulk remove animations from the given addresses and set a constant value to all.
  ///
  /// @param grid_addr_list   a vector of grid addresses to remove animations from.
  /// @param intensity        the constant intensity to set at all provided grid addresses.
  ///
  void remove(std::vector<grid_addr_t>, unsigned int);
  
private:
  /// @brief grid device.
  std::shared_ptr<GridDevice> grid_device;

  /// @breif current layout.
  std::shared_ptr<Layout> current_layout;

  /// @brief mutex for accessinng pixel map.
  std::mutex lock;
  
  /// @brief frame period.
  ///
  /// @remark default is 50 milliseconds.
  std::chrono::milliseconds period = std::chrono::milliseconds(50);

  /// @brief a collection of pixels, each with their own animations.
  ///
  /// @description on each frame, each animation for the next time step is computed
  /// and output to the grid.
  ///
  /// TODO make this even further nested so there is a wrapping map from LayoutName -> pixel animation map!!!
  std::unordered_map<grid_coordinates_t, std::shared_ptr<Animation>, grid_coordinates_hasher> pixels;

  /// @brief renders current animation frame.
  void render(std::chrono::milliseconds);
};

#endif
